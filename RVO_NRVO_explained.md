# RVO and NRVO: Return Value Optimization Explained

## Table of Contents
1. [Introduction](#introduction)
2. [RVO - Return Value Optimization](#rvo---return-value-optimization)
3. [NRVO - Named Return Value Optimization](#nrvo---named-return-value-optimization)
4. [Key Insight: Compiler Optimization, Not Code](#key-insight-compiler-optimization-not-code)
5. [History and Compiler Support](#history-and-compiler-support)
6. [Without RVO/NRVO](#without-rvorvo)
7. [With RVO/NRVO](#with-rvorvo)
8. [How to Control RVO/NRVO](#how-to-control-rvorvo)
9. [Move Semantics as Fallback](#move-semantics-as-fallback)
10. [Best Practices](#best-practices)

---

## Introduction

**RVO (Return Value Optimization) and NRVO (Named Return Value Optimization)** are **automatic compiler optimizations** that eliminate unnecessary copy operations when returning objects from functions.

**CRITICAL: You don't write special code for RVO/NRVO—the compiler applies them automatically!**

---

## RVO - Return Value Optimization

### What is RVO?

**RVO** eliminates the temporary copy when returning an **unnamed temporary object** from a function.

```cpp
class Vector {
    int* data;
    int size;
    
public:
    Vector(int s) : size(s) {
        data = new int[s];
        std::cout << "Constructor\n";
    }
    
    ~Vector() {
        std::cout << "Destructor\n";
        delete[] data;
    }
};

// RVO applies here: returns unnamed temporary
Vector createVector() {
    return Vector(10);  // Temporary created directly in return slot
}

int main() {
    Vector v = createVector();
    return 0;
}
```

### Output WITH RVO (Modern Compilers)
```
Constructor     // Vector created in return slot
Destructor      // v destroyed
```

**Total: 1 Constructor, 1 Destructor**

### Output WITHOUT RVO (Disabled or Old Compiler)
```
Constructor     // Vector(10) created
Constructor     // Temporary COPIED to return value (copy constructor)
Destructor      // Temporary destroyed
Destructor      // Return value destroyed
```

**Total: 2 Constructors, 2 Destructors**

---

## NRVO - Named Return Value Optimization

### What is NRVO?

**NRVO** eliminates the copy when returning a **named local variable** that will never be used again.

```cpp
Vector processVector() {
    Vector result(100);     // Named local variable
    
    // ... do work with result ...
    
    return result;          // Return named variable (NRVO can apply)
}
```

### When NRVO Works

```cpp
// ✓ NRVO applies: same variable returned in all paths
Vector createVector(bool condition) {
    Vector result(10);
    
    if (condition) {
        result.scale(2);
    }
    
    return result;  // Same variable, NRVO can optimize
}

// ✓ NRVO applies: single named return
Vector getVector() {
    Vector v(50);
    v.process();
    return v;  // NRVO optimizes
}
```

### When NRVO Does NOT Work

```cpp
// ✗ Multiple different variables
Vector createVector(bool condition) {
    Vector v1(10);
    Vector v2(20);
    return condition ? v1 : v2;  // Can't know which until runtime - no NRVO
}

// ✗ Different return objects
Vector process() {
    Vector v = helper();  // Different object
    return v;             // No NRVO
}
```

---

## Key Insight: Compiler Optimization, Not Code

### YOU CANNOT CODE RVO/NRVO

```cpp
// ✓ You just write normal code:
Vector createVector() {
    return Vector(10);  // Normal, simple return
}

// ✗ You CANNOT write special code for RVO:
// return Vector(10) in_special_rvo_way();  // This doesn't exist!

// The COMPILER applies RVO automatically
// You have no control in the code itself
```

### What You Control

You control:
1. **How you write the code** (simple and clean)
2. **Compiler optimization flags** (`-O0`, `-O1`, `-O2`, `-O3`)
3. **Disabling RVO** (for debugging or testing)

```cpp
// ✓ Write this (allows optimization):
Vector create() {
    return Vector(10);
}

// ✗ Avoid this (prevents optimization):
Vector create() {
    Vector v(10);
    Vector copy = v;
    return copy;  // Different variable than v
}
```

---

## History and Compiler Support

### Pre-2000s: NO RVO/NRVO

```
Very old compilers:
❌ NO RVO support
❌ NO NRVO support
❌ Every return created a copy
Result: Expensive temporary object copies
```

### 2000-2010: Partial Support

```
Early 2000s compilers:
~ RVO partially supported
❌ NRVO rarely supported
❌ Inconsistent across compilers
Result: Unpredictable performance
```

### 2010+ (C++11): Good Support

```
Modern compilers (GCC, Clang, MSVC):
✓ RVO well supported
✓ NRVO well supported
✓ Consistent behavior
✓ Move semantics as fallback
Result: Predictable, efficient code
```

### Compiler Support Table

| Compiler | RVO | NRVO | Era |
|----------|-----|------|-----|
| Very Old (pre-2000) | ❌ | ❌ | Expensive copies |
| GCC 3.x - 4.x | ~ | ~ | Partial |
| GCC 5+ | ✓ | ✓ | Good |
| Clang 3+ | ✓ | ✓ | Good |
| MSVC 2010+ | ✓ | ✓ | Good |

---

## Without RVO/NRVO

### Program Example

```cpp
#include <iostream>

class Vector {
    int* data;
    int size;
    
public:
    Vector(int s) : size(s) {
        data = new int[s];
        std::cout << "Constructor(" << s << ")\n";
    }
    
    // Move constructor (for non-RVO case)
    Vector(Vector&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        std::cout << "Move Constructor\n";
    }
    
    ~Vector() {
        std::cout << "Destructor\n";
        delete[] data;
    }
};

Vector createVector() {
    return Vector(10);
}

int main() {
    Vector v = createVector();
    Vector v2(5);
    return 0;
}
```

### Compile WITHOUT RVO

```bash
# Disable RVO optimization
clang++ -fno-elide-constructors -o no_rvo.exe program.cpp
./no_rvo.exe
```

### Output WITHOUT RVO

```
Constructor(10)
Move Constructor     ← Move instead of copy (C++11+)
Destructor
Constructor(5)
Destructor
Destructor
```

**Explanation:**
1. `Constructor(10)` - Vector created in `createVector()`
2. `Move Constructor` - Temporary moved to return value (move semantics kicks in)
3. `Destructor` - Temporary destroyed
4. `Constructor(5)` - v2 created
5. `Destructor` - v destroyed
6. `Destructor` - v2 destroyed

---

## With RVO/NRVO

### Same Program WITH RVO

```bash
# Enable RVO (default in modern compilers)
clang++ -O2 -o with_rvo.exe program.cpp
./with_rvo.exe
```

### Output WITH RVO

```
Constructor(10)
Constructor(5)
Destructor
Destructor
```

**Explanation:**
1. `Constructor(10)` - Vector created directly in return slot (RVO!)
2. `Constructor(5)` - v2 created
3. `Destructor` - v destroyed
4. `Destructor` - v2 destroyed

**Difference:** Only 2 constructor/destructor pairs instead of 3!

---

## How to Control RVO/NRVO

### Enable RVO/NRVO (Default)

```bash
# Default with optimization
g++ -O2 program.cpp
g++ -O3 program.cpp

# Clang
clang++ -O2 program.cpp

# MSVC
cl /O2 program.cpp
```

### Disable RVO/NRVO (for Testing/Debugging)

```bash
# GCC/Clang: Disable RVO
g++ -fno-elide-constructors program.cpp
clang++ -fno-elide-constructors program.cpp

# Compiler flags
# -O0: No optimization (RVO unlikely)
# -O1: Some optimization (RVO partial)
# -O2: Good optimization (RVO likely)
# -O3: Full optimization (RVO likely)
```

### Check Compiler Behavior

```cpp
#include <iostream>

class Vector {
    int size;
public:
    Vector(int s) : size(s) {
        std::cout << "Constructor\n";
    }
    ~Vector() {
        std::cout << "Destructor\n";
    }
};

Vector create() {
    return Vector(10);
}

int main() {
    std::cout << "Creating vector...\n";
    Vector v = create();
    std::cout << "Done\n";
    return 0;
}
```

**With RVO:** 2 calls (1 constructor, 1 destructor)
**Without RVO:** 3+ calls (2 constructors, 2+ destructors)

---

## Move Semantics as Fallback

### C++11 Solution: Move Semantics

Even if RVO/NRVO don't apply, **move semantics (C++11+)** provides efficient transfer:

```cpp
class Vector {
private:
    int* data;
    int size;
    
public:
    // Normal constructor
    Vector(int s) : size(s) {
        data = new int[s];
        std::cout << "Constructor\n";
    }
    
    // Move constructor - efficient transfer (C++11+)
    Vector(Vector&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        std::cout << "Move Constructor (FAST!)\n";
    }
    
    // Destructor
    ~Vector() {
        delete[] data;
        std::cout << "Destructor\n";
    }
};
```

### Performance Guarantee Chain

```
C++98 Path (OLD):
copy → copy → copy (expensive)

C++11+ Path (MODERN):
1. Try RVO (best - no copy, no move)
2. Fallback to Move (good - just transfer)
3. Last resort: Copy (acceptable with move available)
```

---

## Best Practices

### 1. Always Return by Value (Don't Overthink)

```cpp
// ✓ GOOD: Let compiler optimize
Vector createVector() {
    return Vector(10);  // RVO or move will handle it
}

// ✗ AVOID: Returning pointers
Vector* createVectorPtr() {
    return new Vector(10);  // Forces manual memory management
}

// ✗ AVOID: Returning references to locals
Vector& createVectorRef() {
    Vector v(10);
    return v;  // Dangling reference!
}
```

### 2. Write Simple, Clear Code

```cpp
// ✓ GOOD: Simple code that compiler can optimize
Vector calculate() {
    Vector result(100);
    result.process();
    return result;  // NRVO can apply
}

// ✗ POOR: Complex code that prevents optimization
Vector calculate() {
    Vector result(100);
    Vector temp = result;
    result.process();
    if (someCondition) return temp;
    return result;  // Can't optimize
}
```

### 3. Use Move Semantics (C++11+)

```cpp
// ✓ GOOD: Define move constructor
class Vector {
public:
    Vector(Vector&& other) noexcept {
        data = other.data;
        other.data = nullptr;
    }
};

// Guarantees performance even if RVO/NRVO don't apply
```

### 4. Use Rule of Zero When Possible

```cpp
// ✓ BEST: Use smart pointers and STL
class Vector {
private:
    std::unique_ptr<int[]> data;  // Automatic optimization
    std::vector<int> items;        // Automatic optimization
    // No destructor, no move constructor needed!
    // Compiler handles everything optimally
};
```

### 5. Trust the Compiler

```cpp
// ✓ GOOD: Trust compiler optimization
Vector create() {
    return Vector(10);
}

// ✗ DON'T: Try to "help" the compiler
Vector create() {
    Vector v(10);
    return std::move(v);  // Unnecessary! Compiler already handles NRVO
}

// Exception: Return multiple values where compiler can't choose
Vector create(bool condition) {
    Vector v1(10);
    Vector v2(20);
    return condition ? std::move(v1) : std::move(v2);  // Move helps here
}
```

### 6. Enable Optimizations

```bash
# Production code: Always enable optimization
g++ -O2 program.cpp   # Good
g++ -O3 program.cpp   # Best

# Development: You can disable for debugging
g++ -O0 -g program.cpp  # No optimization, full debugging
```

---

## Summary

### What You Need to Know

| Concept | What It Is | Your Role |
|---------|-----------|-----------|
| **RVO** | Compiler eliminates temp copy on unnamed returns | Write normal code |
| **NRVO** | Compiler eliminates copy on named variable return | Write normal code |
| **Move** | Transfer of resources (C++11+) | Define move constructor |
| **Your Code** | Return by value, keep it simple | Trust compiler |
| **Compiler Flags** | Control optimization level | Use `-O2` or `-O3` |

### Quick Checklist

```cpp
// ✓ DO THIS:
1. Return objects by value
2. Write simple, clear code
3. Define move constructors (C++11+)
4. Enable compiler optimizations (-O2, -O3)
5. Use smart pointers and STL

// ✗ DON'T DO THIS:
1. Return pointers/references to locals
2. Write complex code to "help" compiler
3. Use std::move unnecessarily with NRVO
4. Disable optimizations in production
5. Try to manually code RVO/NRVO
```

### The Golden Rule

**Write clean, simple code that returns objects by value. The compiler will automatically apply RVO, NRVO, and move semantics to optimize it. You don't need to do anything special.**

---

## Conclusion

- **RVO/NRVO are COMPILER OPTIMIZATIONS**, not something you code
- **Modern compilers apply them automatically**
- **Move semantics provides a performance guarantee** even when RVO/NRVO don't apply
- **Your job**: Write clean code and trust the compiler
- **Compiler's job**: Optimize return statements efficiently

Return by value, keep code simple, enable optimizations—the rest happens automatically!
