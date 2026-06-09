#include "matrix_v3.h"
#include <iomanip>
#include <cmath>

// Initialize static members
int MatrixV3::instance_count = 0;
int MatrixV3::copy_count = 0;
int MatrixV3::move_count = 0;

// Constructor
MatrixV3::MatrixV3(int rows, int cols)
    : rows_(rows), cols_(cols) {
    
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    
    int size = rows * cols;
    data = new double[size];
    
    std::memset(data, 0, size * sizeof(double));
    
    instance_count++;
}

// Destructor
MatrixV3::~MatrixV3() {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }
    instance_count--;
}

// Copy Constructor
MatrixV3::MatrixV3(const MatrixV3& other)
    : rows_(other.rows_), cols_(other.cols_) {
    
    int size = rows_ * cols_;
    data = new double[size];
    
    std::memcpy(data, other.data, size * sizeof(double));
    
    instance_count++;
    copy_count++;
}

// Copy Assignment Operator
MatrixV3& MatrixV3::operator=(const MatrixV3& other) {
    if (this == &other) {
        return *this;
    }
    
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        delete[] data;
        
        rows_ = other.rows_;
        cols_ = other.cols_;
        int size = rows_ * cols_;
        data = new double[size];
    }
    
    int size = rows_ * cols_;
    std::memcpy(data, other.data, size * sizeof(double));
    
    copy_count++;
    return *this;
}

// Move Constructor
MatrixV3::MatrixV3(MatrixV3&& other) noexcept
    : data(other.data), rows_(other.rows_), cols_(other.cols_) {
    
    other.data = nullptr;
    other.rows_ = 0;
    other.cols_ = 0;
    
    instance_count++;
    move_count++;
}

// Move Assignment Operator
MatrixV3& MatrixV3::operator=(MatrixV3&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    
    delete[] data;
    
    data = other.data;
    rows_ = other.rows_;
    cols_ = other.cols_;
    
    other.data = nullptr;
    other.rows_ = 0;
    other.cols_ = 0;
    
    move_count++;
    return *this;
}

// Element Access
double& MatrixV3::operator()(int row, int col) {
    return data[index(row, col)];
}

const double& MatrixV3::operator()(int row, int col) const {
    return data[index(row, col)];
}

double* MatrixV3::operator[](int row) {
    if (row < 0 || row >= rows_) {
        throw std::out_of_range("Row index out of bounds");
    }
    return &data[row * cols_];
}

const double* MatrixV3::operator[](int row) const {
    if (row < 0 || row >= rows_) {
        throw std::out_of_range("Row index out of bounds");
    }
    return &data[row * cols_];
}

double& MatrixV3::at(int row, int col) {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index(row, col)];
}

const double& MatrixV3::at(int row, int col) const {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index(row, col)];
}

// Addition
MatrixV3 MatrixV3::operator+(const MatrixV3& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match");
    }
    
    MatrixV3 result(rows_, cols_);
    result += *this;
    result += other;
    
    return result;
}

// Subtraction
MatrixV3 MatrixV3::operator-(const MatrixV3& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match");
    }
    
    MatrixV3 result(rows_, cols_);
    result += *this;
    result -= other;
    
    return result;
}

// Multiplication
MatrixV3 MatrixV3::operator*(const MatrixV3& other) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Inner dimensions must match");
    }
    
    MatrixV3 result(rows_, other.cols_);
    
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < other.cols_; ++j) {
            double sum = 0.0;
            for (int k = 0; k < cols_; ++k) {
                sum += data[index(i, k)] * other.data[other.index(k, j)];
            }
            result.data[result.index(i, j)] = sum;
        }
    }
    
    return result;
}

// Compound Addition (NEW in V3)
MatrixV3& MatrixV3::operator+=(const MatrixV3& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match");
    }
    
    int size = rows_ * cols_;
    for (int i = 0; i < size; ++i) {
        data[i] += other.data[i];
    }
    
    return *this;
}

// Compound Subtraction (NEW in V3)
MatrixV3& MatrixV3::operator-=(const MatrixV3& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match");
    }
    
    int size = rows_ * cols_;
    for (int i = 0; i < size; ++i) {
        data[i] -= other.data[i];
    }
    
    return *this;
}

// Equality
bool MatrixV3::operator==(const MatrixV3& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        return false;
    }
    
    const double EPSILON = 1e-9;
    int size = rows_ * cols_;
    
    for (int i = 0; i < size; ++i) {
        if (std::abs(data[i] - other.data[i]) > EPSILON) {
            return false;
        }
    }
    
    return true;
}

// Fill with value
void MatrixV3::fill(double value) {
    int size = rows_ * cols_;
    for (int i = 0; i < size; ++i) {
        data[i] = value;
    }
}

// Statistics
void MatrixV3::resetStats() {
    instance_count = 0;
    copy_count = 0;
    move_count = 0;
}

void MatrixV3::printStats() {
    std::cout << "MatrixV3 Statistics:\n"
              << "  Instance Count: " << instance_count << "\n"
              << "  Copy Count:     " << copy_count << "\n"
              << "  Move Count:     " << move_count << "\n";
}

int MatrixV3::getInstanceCount() { return instance_count; }
int MatrixV3::getCopyCount() { return copy_count; }
int MatrixV3::getMoveCount() { return move_count; }

// Stream Output
std::ostream& operator<<(std::ostream& os, const MatrixV3& mat) {
    os << "MatrixV3 " << mat.rows() << "x" << mat.cols() << ":\n";
    for (int i = 0; i < mat.rows(); ++i) {
        os << "  [ ";
        for (int j = 0; j < mat.cols(); ++j) {
            os << std::fixed << std::setw(8) << std::setprecision(2)
               << mat(i, j) << " ";
        }
        os << "]\n";
    }
    return os;
}
