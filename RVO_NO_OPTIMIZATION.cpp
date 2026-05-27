#include <iostream>

class Vector {
    int* data;
    int size;
    
public:
    Vector(int s) : size(s) {
        data = new int[s];
        std::cout << "Constructor(" << s << ")\n";
    }
    
    // Move constructor - called when RVO is disabled
    Vector(Vector&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        std::cout << "Move Constructor\n";
    }
    
    ~Vector() {
        std::cout << "Destructor\n";
        delete[] data;
    }
};

// Without RVO (without -fno-elide-constructors):
// 1. Create temporary Vector(10) inside function
// 2. Move temporary to return value (move constructor called)
// 3. Destroy temporary
// 4. Caller receives the moved object

Vector createVector() {
    return Vector(10);  // Without RVO: temporary -> move -> destroy
}

int main() {
    std::cout << "=== WITHOUT RVO (using -fno-elide-constructors) ===\n\n";
    
    std::cout << "Calling createVector():\n";
    Vector v = createVector();
    
    std::cout << "\nDirect construction:\n";
    Vector v2(5);
    
    std::cout << "\nEnd of main - destroying objects:\n";
    // v destroyed
    // v2 destroyed
    
    return 0;
}
