# Complete Guide to Operator Overloading in C++

## Table of Contents
1. [Introduction](#introduction)
2. [Member vs Non-Member Operators](#member-vs-non-member-operators)
3. [Operators You Can Overload](#operators-you-can-overload)
4. [Operators You Cannot Overload](#operators-you-cannot-overload)
5. [Detailed Examples](#detailed-examples)
6. [Rules of Three, Five, and Zero](#rules-of-three-five-and-zero)
7. [Best Practices](#best-practices)

---

## Introduction

**Operator overloading** allows you to define custom behavior for operators when used with user-defined types (classes/structs). It makes your classes more intuitive and easier to use.

```cpp
class Complex {
    double real, imag;
};

Complex a{1, 2}, b{3, 4};
Complex c = a + b;  // Uses overloaded + operator
```

---

## Member vs Non-Member Operators

### Member Operators

**Defined inside the class** as member functions:

```cpp
class Complex {
private:
    double real, imag;
    
public:
    // Member operator: left operand is implicit 'this'
    Complex operator+(const Complex& other) const {
        return Complex{real + other.real, imag + other.imag};
    }
};

// Usage: a + b becomes a.operator+(b)
Complex a{1, 2}, b{3, 4};
Complex c = a + b;  // ✓ Works
```

**Characteristics:**
- Have implicit `this` pointer (always the **left operand**)
- Only need one explicit parameter (right operand)
- Called on an object: `a.operator+(b)`

### Non-Member Operators

**Defined outside the class** as standalone functions:

```cpp
class Complex {
private:
    double real, imag;
    
public:
    // Friend allows access to private members
    friend Complex operator+(const Complex& a, const Complex& b);
};

// Non-member function
Complex operator+(const Complex& a, const Complex& b) {
    return Complex{a.real + b.real, a.imag + b.imag};
}

// Usage: a + b becomes operator+(a, b)
Complex a{1, 2}, b{3, 4};
Complex c = a + b;  // ✓ Works
```

**Characteristics:**
- Both operands are explicit parameters
- **Enables symmetric operations** (left operand can be different type)
- Called as function: `operator+(a, b)`

### Why Non-Member for Symmetric Operations?

```cpp
class Complex {
public:
    // Member: only works as (Complex + double)
    Complex operator+(double d) const {
        return Complex{real + d, imag};
    }
};

// This FAILS:
Complex c = 2.0 + Complex{1, 2};  // Error! double has no operator+(Complex)
```

**Solution: Use non-member functions**

```cpp
Complex operator+(const Complex& c, double d) {
    return Complex{c.real + d, c.imag};
}

Complex operator+(double d, const Complex& c) {
    return Complex{d + c.real, c.imag};
}

// Both work:
Complex c1 = Complex{1, 2} + 3.0;  // ✓
Complex c2 = 3.0 + Complex{1, 2};  // ✓
```

---

## Operators You Can Overload

### Arithmetic Operators
```cpp
Complex operator+(const Complex& other) const;  // Addition
Complex operator-(const Complex& other) const;  // Subtraction
Complex operator*(const Complex& other) const;  // Multiplication
Complex operator/(const Complex& other) const;  // Division
int operator%(int other) const;                 // Modulo
Complex operator+() const;                      // Unary plus
Complex operator-() const;                      // Unary minus
```

### Comparison Operators (should be non-member)
```cpp
bool operator==(const Complex& other) const;
bool operator!=(const Complex& other) const;
bool operator<(const Complex& other) const;
bool operator>(const Complex& other) const;
bool operator<=(const Complex& other) const;
bool operator>=(const Complex& other) const;
```

### Logical Operators
```cpp
bool operator&&(const Complex& other) const;
bool operator||(const Complex& other) const;
bool operator!() const;
```

### Assignment Operators (MUST be member)
```cpp
Complex& operator=(const Complex& other);       // Copy assignment
Complex& operator+=(const Complex& other);
Complex& operator-=(const Complex& other);
Complex& operator*=(const Complex& other);
Complex& operator/=(const Complex& other);
```

### Increment/Decrement Operators (MUST be member)
```cpp
Complex& operator++();        // Prefix ++
Complex operator++(int);      // Postfix ++ (int dummy parameter)
Complex& operator--();        // Prefix --
Complex operator--(int);      // Postfix -- (int dummy parameter)
```

### Subscript Operator (MUST be member)
```cpp
double& operator[](int index);
double operator[](int index) const;
```

### Function Call Operator (MUST be member)
```cpp
int operator()(int a, int b) const;

// Usage: object_instance(args)
```

### Member Access Operators (MUST be member)
```cpp
double* operator->();         // Arrow operator
double& operator*();           // Dereference operator
```

### Stream Operators (MUST be non-member)
```cpp
std::ostream& operator<<(std::ostream& os, const Complex& c);
std::istream& operator>>(std::istream& is, Complex& c);
```

### Other Operators
```cpp
Complex operator,(const Complex& other);  // Comma
Complex operator&();                      // Address-of (rarely overloaded)
```

---

## Operators You Cannot Overload

These operators **CANNOT** be overloaded:

```cpp
::    // Scope resolution
.     // Member selector
.*    // Member pointer selector
?:    // Ternary conditional
sizeof() // Size of
typeid() // Type identification
#     // Preprocessor
##    // Preprocessor concatenation
```

---

## Detailed Examples

### Example 1: Simple Complex Number Class

```cpp
#include <iostream>
#include <cmath>

class Complex {
private:
    double real, imag;
    
public:
    // Constructors
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    
    // Getters
    double getReal() const { return real; }
    double getImag() const { return imag; }
    
    // Member operators
    Complex operator+(const Complex& other) const {
        return Complex{real + other.real, imag + other.imag};
    }
    
    Complex operator-(const Complex& other) const {
        return Complex{real - other.real, imag - other.imag};
    }
    
    Complex operator*(const Complex& other) const {
        double r = real * other.real - imag * other.imag;
        double i = real * other.imag + imag * other.real;
        return Complex{r, i};
    }
    
    Complex& operator+=(const Complex& other) {
        real += other.real;
        imag += other.imag;
        return *this;
    }
    
    Complex& operator++() {  // Prefix
        real++;
        return *this;
    }
    
    Complex operator++(int) {  // Postfix
        Complex temp = *this;
        real++;
        return temp;
    }
    
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }
    
    // Friend functions for non-member operators
    friend Complex operator+(double d, const Complex& c);
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
};

// Non-member operators
Complex operator+(double d, const Complex& c) {
    return Complex{d + c.real, c.imag};
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.real;
    if (c.imag >= 0) os << " + ";
    os << c.imag << "i";
    return os;
}

// Usage
int main() {
    Complex a{3, 4};
    Complex b{1, 2};
    
    Complex c = a + b;           // Member + operator
    std::cout << c << std::endl; // Output: 4 + 6i
    
    Complex d = 2.0 + a;         // Non-member + operator
    std::cout << d << std::endl; // Output: 5 + 4i
    
    a += b;                        // += operator
    std::cout << a << std::endl; // Output: 4 + 6i
    
    ++a;                           // Prefix ++
    std::cout << a << std::endl; // Output: 5 + 6i
    
    return 0;
}
```

### Example 2: Vector Class with Subscript

```cpp
#include <iostream>
#include <stdexcept>

class Vector {
private:
    double* data;
    int size;
    
public:
    Vector(int s) : size(s) {
        data = new double[size]{};
    }
    
    ~Vector() { delete[] data; }
    
    // Copy constructor (Rule of Five)
    Vector(const Vector& other) : size(other.size) {
        data = new double[size];
        std::copy(other.data, other.data + size, data);
    }
    
    // Copy assignment
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new double[size];
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }
    
    // Move constructor
    Vector(Vector&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }
    
    // Move assignment
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
    
    // Subscript operator
    double& operator[](int index) {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        return data[index];
    }
    
    double operator[](int index) const {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        return data[index];
    }
    
    int getSize() const { return size; }
};

// Usage
int main() {
    Vector v(3);
    v[0] = 1.0;
    v[1] = 2.5;
    v[2] = 3.7;
    
    std::cout << v[1] << std::endl;  // Output: 2.5
    
    Vector v2 = v;  // Copy constructor
    v2[0] = 99.0;
    
    std::cout << v[0] << std::endl;  // Output: 1 (unchanged)
    std::cout << v2[0] << std::endl; // Output: 99
    
    return 0;
}
```

### Example 3: Function Object (Callable Class)

```cpp
#include <iostream>

class Multiplier {
private:
    int factor;
    
public:
    Multiplier(int f) : factor(f) {}
    
    // Function call operator
    int operator()(int x) const {
        return x * factor;
    }
};

// Usage
int main() {
    Multiplier double_it(2);
    Multiplier triple_it(3);
    
    std::cout << double_it(5) << std::endl;  // Output: 10
    std::cout << triple_it(5) << std::endl;  // Output: 15
    
    // Can be used with algorithms
    int arr[] = {1, 2, 3, 4, 5};
    std::transform(arr, arr + 5, arr, double_it);
    
    return 0;
}
```

---

## Rules of Three, Five, and Zero

### Rule of Three (C++98)

If you define **any one** of these three, define **all three**:

1. **Destructor** - releases resources
2. **Copy Constructor** - deep copy on object creation
3. **Copy Assignment Operator** - deep copy on assignment

```cpp
class String {
private:
    char* data;
    int size;
    
public:
    // Constructor
    String(const char* s = "") {
        size = std::strlen(s);
        data = new char[size + 1];
        std::strcpy(data, s);
    }
    
    // 1. Destructor
    ~String() {
        delete[] data;
    }
    
    // 2. Copy Constructor
    String(const String& other) {
        size = other.size;
        data = new char[size + 1];
        std::strcpy(data, other.data);
    }
    
    // 3. Copy Assignment Operator
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new char[size + 1];
            std::strcpy(data, other.data);
        }
        return *this;
    }
};
```

### Rule of Five (C++11)

If you define **any one**, define **all five** (adds move operations):

```cpp
class String {
private:
    char* data;
    int size;
    
public:
    // Original three
    ~String() { delete[] data; }
    
    String(const String& other) { /* deep copy */ }
    
    String& operator=(const String& other) { /* deep copy */ }
    
    // 4. Move Constructor - transfer ownership (cheap operation)
    String(String&& other) noexcept {
        data = other.data;
        size = other.size;
        other.data = nullptr;  // Leave source empty
        other.size = 0;
    }
    
    // 5. Move Assignment Operator
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
};

// Usage
String s1("hello");
String s2 = std::move(s1);  // Move constructor (fast)
String s3 = s1;              // Copy constructor (slower)
```

### Rule of Zero (Modern C++)

**If you don't manage resources, define NONE** of these functions.

```cpp
class Point {
private:
    int x, y;
    std::string name;  // std::string manages its own memory
    std::vector<int> values;  // std::vector manages its own memory
    
public:
    // No destructor, constructors, or operators needed!
    // Compiler generates all five automatically
};

Point p1{1, 2};
Point p2 = p1;              // Auto-generated copy works perfectly
Point p3 = std::move(p1);   // Auto-generated move works perfectly
```

**Benefits:**
- Simpler code
- Compiler-generated versions are typically optimal
- No resource leaks
- Modern containers handle everything

---

## Best Practices

### 1. **Use Non-Member Functions for Symmetric Operators**

```cpp
// ✓ Good: symmetric operations
Complex operator+(const Complex& a, const Complex& b);
bool operator==(const Complex& a, const Complex& b);
Complex operator*(double d, const Complex& c);

// ✗ Avoid: member functions for symmetric ops
// Complex operator+(const Complex& other); // Can't do d + complex
```

### 2. **Pass Arguments by const Reference**

```cpp
// ✓ Good: avoids unnecessary copies
Complex operator+(const Complex& a, const Complex& b);

// ✗ Avoid: copies parameters
Complex operator+(Complex a, Complex b);
```

### 3. **Return by Value for Arithmetic, by Reference for Assignment**

```cpp
// ✓ Good: arithmetic returns new object
Complex operator+(const Complex& a, const Complex& b);

// ✓ Good: assignment returns reference for chaining
Complex& operator=(const Complex& other);

// ✗ Avoid: returning reference for arithmetic (dangling reference)
Complex& operator+(const Complex& other);
```

### 4. **Implement Compound Assignment Operators Efficiently**

```cpp
// ✓ Good: efficient implementation
Complex& operator+=(const Complex& other) {
    real += other.real;
    imag += other.imag;
    return *this;
}

// Use it to implement simple addition
Complex operator+(const Complex& a, const Complex& b) {
    return Complex(a) += b;
}
```

### 5. **Use `friend` for Non-Member Access**

```cpp
class Complex {
private:
    double real, imag;
    friend Complex operator+(const Complex& a, const Complex& b);
};

Complex operator+(const Complex& a, const Complex& b) {
    return Complex{a.real + b.real, a.imag + b.imag};  // Access private
}
```

### 6. **Implement Copy/Move Correctly (Rule of Five)**

```cpp
// ✓ Pattern for Rule of Five
class MyClass {
public:
    ~MyClass() { /* cleanup */ }
    MyClass(const MyClass& other) { /* deep copy */ }
    MyClass& operator=(const MyClass& other) { /* deep copy */ }
    MyClass(MyClass&& other) noexcept { /* steal resources */ }
    MyClass& operator=(MyClass&& other) noexcept { /* steal resources */ }
};

// ✓ Or use Rule of Zero (preferred for modern C++)
class MyClass {
private:
    std::unique_ptr<int> data;
    std::vector<int> items;
    // Compiler handles all five automatically
};
```

### 7. **Mark const Correctness**

```cpp
// ✓ Good: const for operations that don't modify
bool operator==(const Complex& other) const;
double operator[](int index) const;

// ✗ Wrong: should be const
bool operator==(const Complex& other);  // Should be const
```

### 8. **Avoid Overloading `operator,`, `operator&`, `operator()`  Unless Necessary**

These operators are rarely overloaded as they can break intuitive behavior.

### 9. **Check for Self-Assignment in Copy Assignment**

```cpp
String& operator=(const String& other) {
    if (this != &other) {  // Prevent self-assignment problems
        delete[] data;
        // ... copy logic
    }
    return *this;
}
```

### 10. **Prefer Using Standard Library Types**

```cpp
// ✓ Use smart pointers instead of raw pointers
class MyClass {
    std::unique_ptr<int> data;  // No manual destructor needed
};

// ✓ Use STL containers instead of manual arrays
class MyClass {
    std::vector<int> items;  // No manual destructor needed
};
```

---

## Summary Table

| Operator | Type | Example |
|----------|------|---------|
| Arithmetic `+`, `-`, `*`, `/` | Non-member | `operator+(a, b)` |
| Comparison `==`, `<`, etc. | Non-member | `operator==(a, b)` |
| Assignment `=`, `+=`, etc. | Member | `a.operator=(b)` |
| Increment/Decrement `++`, `--` | Member | `a.operator++()` |
| Subscript `[]` | Member | `a[i]` |
| Function call `()` | Member | `a(args)` |
| Stream `<<`, `>>` | Non-member | `os << a` |
| Dereference `*`, `->` | Member | `*ptr`, `ptr->member` |
| Unary `-`, `!`, `~` | Member or Non-member | varies |

---

## Key Takeaways

1. **Member vs Non-Member**: Use non-member for symmetric operations, member when required
2. **Rule of Five**: Define all five or none (use smart pointers and STL for "none")
3. **Const Correctness**: Mark operations that don't modify as `const`
4. **Efficiency**: Pass by `const&`, return by value for temporaries, by reference for assignment
5. **Readability**: Only overload operators when it makes code clearer
6. **Modern C++**: Prefer `Rule of Zero` with smart pointers and containers
