# Rule of Five and Expression Templates (Operator Overloading Deep Dive)

## Why these two topics belong together

When you design numeric or container-like C++ types with overloaded operators (`+`, `-`, `*`, `[]`, etc.), two advanced concerns appear quickly:

1. **Object lifetime and ownership correctness** (Rule of Five)
2. **Temporary-object performance costs** (Expression Templates)

The Rule of Five keeps your type safe and correct.
Expression templates keep your type fast for chained arithmetic expressions.

---

## 1. Rule of Five in Depth

### Core idea

If your class manually manages a resource (heap memory, file handle, socket, mutex wrapper, etc.), and you need one of these special member functions, you usually need all five:

1. Destructor
2. Copy constructor
3. Copy assignment operator
4. Move constructor
5. Move assignment operator

This is called the **Rule of Five**.

### Why this matters for operator overloading

Operators often create and return objects by value. That means copies/moves happen frequently.
If copy/move behavior is wrong, overloaded operators can cause:

- double frees
- memory leaks
- use-after-free
- expensive unnecessary deep copies

### A minimal owning type example

```cpp
#include <algorithm>
#include <cstddef>
#include <utility>

class Vector {
private:
    std::size_t n_ = 0;
    double* data_ = nullptr;

public:
    // 1) Destructor
    ~Vector() {
        delete[] data_;
    }

    explicit Vector(std::size_t n) : n_(n), data_(n ? new double[n] : nullptr) {}

    // 2) Copy constructor (deep copy)
    Vector(const Vector& other) : n_(other.n_), data_(other.n_ ? new double[other.n_] : nullptr) {
        std::copy(other.data_, other.data_ + n_, data_);
    }

    // 3) Copy assignment (deep copy, strong exception safety via copy-and-swap)
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other);
            swap(temp);
        }
        return *this;
    }

    // 4) Move constructor (steal resource)
    Vector(Vector&& other) noexcept : n_(other.n_), data_(other.data_) {
        other.n_ = 0;
        other.data_ = nullptr;
    }

    // 5) Move assignment (release own resource, steal other's)
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            n_ = other.n_;
            data_ = other.data_;
            other.n_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    void swap(Vector& other) noexcept {
        std::swap(n_, other.n_);
        std::swap(data_, other.data_);
    }

    std::size_t size() const { return n_; }
    double& operator[](std::size_t i) { return data_[i]; }
    const double& operator[](std::size_t i) const { return data_[i]; }
};
```

### Design notes

- `noexcept` on move operations is important. Containers like `std::vector` can optimize relocation if move is `noexcept`.
- Copy assignment uses copy-and-swap to provide strong exception safety.
- If ownership is unique, copying may be deleted intentionally.

### Operator overloading with this type

```cpp
Vector operator+(const Vector& a, const Vector& b) {
    Vector out(a.size());
    for (std::size_t i = 0; i < a.size(); ++i) {
        out[i] = a[i] + b[i];
    }
    return out; // NRVO/move
}
```

This is correct, but chained expressions like `a + b + c + d` still create multiple temporaries.

---

## 2. From Rule of Five to Rule of Zero

In modern C++, prefer **Rule of Zero** when possible:

- store resources in standard RAII types (`std::vector`, `std::string`, `std::unique_ptr`)
- let compiler-generated special members do the work

Example: if `Vector` stores `std::vector<double> data_;`, you usually do not write any of the five manually.

This is safer and usually faster to develop.

---

## 3. Expression Templates in Depth

### The performance problem

Naive overloaded operators often allocate temporaries:

```cpp
Vector z = a + b + c;
```

Naively, this becomes roughly:

1. `t1 = a + b`
2. `t2 = t1 + c`
3. `z = t2`

This can mean extra allocations, copies, and memory bandwidth.

### Expression template idea

Instead of computing immediately, build a lightweight expression tree at compile time, then evaluate once in assignment.

- `operator+` returns an expression node (no allocation, no loop yet)
- `Vector::operator=(Expr)` runs one loop and computes each element on demand

### A compact example

```cpp
#include <cstddef>
#include <vector>

// Base expression interface (CRTP style)
template <typename E>
struct Expr {
    double operator[](std::size_t i) const {
        return static_cast<const E&>(*this)[i];
    }
    std::size_t size() const {
        return static_cast<const E&>(*this).size();
    }
};

class VectorET : public Expr<VectorET> {
private:
    std::vector<double> data_;

public:
    explicit VectorET(std::size_t n) : data_(n) {}

    std::size_t size() const { return data_.size(); }
    double operator[](std::size_t i) const { return data_[i]; }
    double& operator[](std::size_t i) { return data_[i]; }

    template <typename E>
    VectorET& operator=(const Expr<E>& exprBase) {
        const E& expr = static_cast<const E&>(exprBase);
        for (std::size_t i = 0; i < data_.size(); ++i) {
            data_[i] = expr[i]; // single fused loop
        }
        return *this;
    }
};

// Expression node representing lhs + rhs
template <typename L, typename R>
class AddExpr : public Expr<AddExpr<L, R>> {
private:
    const L& lhs_;
    const R& rhs_;

public:
    AddExpr(const L& lhs, const R& rhs) : lhs_(lhs), rhs_(rhs) {}

    std::size_t size() const { return lhs_.size(); }
    double operator[](std::size_t i) const { return lhs_[i] + rhs_[i]; }
};

// Generic operator+ for expressions
template <typename L, typename R>
AddExpr<L, R> operator+(const Expr<L>& lhs, const Expr<R>& rhs) {
    return AddExpr<L, R>(static_cast<const L&>(lhs), static_cast<const R&>(rhs));
}
```

Usage:

```cpp
VectorET a(1000), b(1000), c(1000), z(1000);
z = a + b + c; // builds expression tree, evaluates once
```

### Why this is fast

The final loop can look like:

```cpp
for (i) z[i] = a[i] + b[i] + c[i];
```

No intermediate `Vector` allocations in the arithmetic chain.

### Caveats

- More template complexity and harder compiler errors.
- Aliasing issues (example: `a = a + b`) may require special handling.
- Debugging generated code is harder.
- Compile times can increase.

---

## 4. Combining Both Correctly

A robust numeric class typically does this:

1. Uses Rule of Zero where possible (`std::vector` storage)
2. Implements expression templates for arithmetic-heavy paths
3. Keeps move operations `noexcept` if custom ownership exists
4. Provides clear dimension checks and assertions

Recommended practical strategy:

1. Start with clear, correct non-template operators.
2. Profile real workloads.
3. Introduce expression templates only if temporaries are a measured bottleneck.

---

## 5. Checklist for Your Operator-Overloaded Types

- Does the type own a raw resource?
- If yes, did you define all five special members (or delete copy intentionally)?
- Are moves marked `noexcept` when valid?
- Are arithmetic operators dimension-safe?
- Do chained expressions create too many temporaries?
- If yes, would expression templates provide measurable wins?

---

## 6. Key Takeaways

- The **Rule of Five** is about correctness and ownership semantics.
- **Expression templates** are about performance of overloaded arithmetic operators.
- Correctness comes first, then optimization.
- In modern C++, prefer Rule of Zero unless you truly need manual resource ownership.

---

## Further reading topics

- Rule of Zero and RAII design
- Copy elision, NRVO, and move semantics interaction
- BLAS-style kernels and SIMD backends for numeric types
- Libraries using expression templates (Eigen, Blaze, xtensor)
