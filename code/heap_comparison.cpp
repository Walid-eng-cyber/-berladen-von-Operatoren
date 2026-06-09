// V1 vs V2 Heap Usage Profiler
// Compares memory allocation patterns between V1 (copy semantics) and V2 (move semantics)

#include "matrix.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>

using namespace std;

void resetCounters() {
    Matrix::resetStats();
}

void printHeapStats(const string& label, int operations) {
    cout << "\n" << label << " - " << operations << " operations:\n";
    Matrix::printStats();
}

int main() {
    cout << "=== V1 vs V2 Heap Usage Comparison ===\n\n";
    
    // ============= Test 1: Simple Addition =============
    resetCounters();
    {
        Matrix m1(100, 100), m2(100, 100), m3(100, 100);
        m1.fill(1.0);
        m2.fill(2.0);
        
        // V1 style: Creates temporary (copy semantics)
        m3 = m1 + m2;
    }
    printHeapStats("V1 - Simple Addition", 1);
    
    // ============= Test 2: Chained Addition =============
    resetCounters();
    {
        Matrix m1(100, 100), m2(100, 100), m3(100, 100), m4(100, 100);
        m1.fill(1.0);
        m2.fill(2.0);
        m3.fill(3.0);
        
        // V2 style: With move semantics, reduces copies
        m4 = m1 + m2 + m3;  // Without move: m1+m2 -> temp, temp+m3 -> result
    }
    printHeapStats("V2 - Chained Addition", 1);
    
    // ============= Test 3: Loop with Accumulation =============
    cout << "\n--- Test 3: Accumulation Loop (100 iterations) ---\n";
    resetCounters();
    {
        Matrix result(100, 100);
        result.fill(0.0);
        
        for (int i = 0; i < 100; i++) {
            Matrix temp(100, 100);
            temp.fill(1.0 / 100);
            result += temp;  // V3: In-place addition
        }
    }
    cout << "Accumulation with += :\n";
    Matrix::printStats();
    
    // ============= Test 4: Vector Operations =============
    cout << "\n--- Test 4: Vector of Matrices (100 matrices) ---\n";
    resetCounters();
    {
        vector<Matrix> matrices;
        
        // Without move semantics: all matrices would be copied into vector
        for (int i = 0; i < 100; i++) {
            Matrix m(100, 100);
            m.fill((double)i);
            matrices.push_back(m);  // With move: efficient, with copy: expensive
        }
    }
    cout << "Vector operations:\n";
    Matrix::printStats();
    
    // ============= Test 5: Temporary Expression =============
    cout << "\n--- Test 5: Multiple Temporary Expressions (1000 ops) ---\n";
    resetCounters();
    {
        Matrix m1(50, 50), m2(50, 50), m3(50, 50);
        m1.fill(1.0);
        m2.fill(2.0);
        m3.fill(3.0);
        
        Matrix result(50, 50);
        for (int i = 0; i < 1000; i++) {
            result = m1 + m2 + m3;  // Creates temporaries
        }
    }
    printHeapStats("Temporary Expressions", 1000);
    
    // ============= Summary =============
    cout << "\n=== Summary ===\n";
    cout << "V1 (without move semantics):\n";
    cout << "  - Every temporary creates a full data copy\n";
    cout << "  - m1 + m2 + m3: 2 temporary copies\n";
    cout << "  - High memory pressure in loops\n\n";
    
    cout << "V2 (with move semantics):\n";
    cout << "  - Temporaries steal data pointers (O(1))\n";
    cout << "  - m1 + m2 + m3: Same logic but faster\n";
    cout << "  - Reduced heap allocations and deallocations\n";
    
    return 0;
}
