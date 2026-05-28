
#include <iostream>

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

// Without RVO (conceptually what would happen without optimization):
// 1. Create temporary inside function
// 2. Copy temporary to return location
// 3. Destroy temporary
// 4. Caller receives the copy

// ✓ WITH RVO: Compiler skips the copy
Vector createVector() {
    return Vector(10);  // Temporary created directly in return location
}

int main() {
    Vector v = createVector(); 
    
     // No copy! RVO eliminates intermediate copy
    // Result: 1 constructor, 1 destructor (much faster!)
    return 0;
}