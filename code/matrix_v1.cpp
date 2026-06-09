#include "matrix_v1.h"
#include <iomanip>
#include <cmath>

// Initialize static members
int MatrixV1::instance_count = 0;
int MatrixV1::copy_count = 0;
int MatrixV1::move_count = 0;

// Constructor
MatrixV1::MatrixV1(int rows, int cols)
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
MatrixV1::~MatrixV1() {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }
    instance_count--;
}

// Copy Constructor
MatrixV1::MatrixV1(const MatrixV1& other)
    : rows_(other.rows_), cols_(other.cols_) {
    
    int size = rows_ * cols_;
    data = new double[size];
    
    std::memcpy(data, other.data, size * sizeof(double));
    
    instance_count++;
    copy_count++;
}

// Copy Assignment Operator
MatrixV1& MatrixV1::operator=(const MatrixV1& other) {
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

// Element Access
double& MatrixV1::operator()(int row, int col) {
    return data[index(row, col)];
}

const double& MatrixV1::operator()(int row, int col) const {
    return data[index(row, col)];
}

double* MatrixV1::operator[](int row) {
    if (row < 0 || row >= rows_) {
        throw std::out_of_range("Row index out of bounds");
    }
    return &data[row * cols_];
}

const double* MatrixV1::operator[](int row) const {
    if (row < 0 || row >= rows_) {
        throw std::out_of_range("Row index out of bounds");
    }
    return &data[row * cols_];
}

double& MatrixV1::at(int row, int col) {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index(row, col)];
}

const double& MatrixV1::at(int row, int col) const {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index(row, col)];
}

// Addition
MatrixV1 MatrixV1::operator+(const MatrixV1& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match");
    }
    
    MatrixV1 result(rows_, cols_);
    int size = rows_ * cols_;
    
    for (int i = 0; i < size; ++i) {
        result.data[i] = data[i] + other.data[i];
    }
    
    return result;
}

// Subtraction
MatrixV1 MatrixV1::operator-(const MatrixV1& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match");
    }
    
    MatrixV1 result(rows_, cols_);
    int size = rows_ * cols_;
    
    for (int i = 0; i < size; ++i) {
        result.data[i] = data[i] - other.data[i];
    }
    
    return result;
}

// Multiplication
MatrixV1 MatrixV1::operator*(const MatrixV1& other) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Inner dimensions must match");
    }
    
    MatrixV1 result(rows_, other.cols_);
    
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

// Equality
bool MatrixV1::operator==(const MatrixV1& other) const {
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
void MatrixV1::fill(double value) {
    int size = rows_ * cols_;
    for (int i = 0; i < size; ++i) {
        data[i] = value;
    }
}

// Statistics
void MatrixV1::resetStats() {
    instance_count = 0;
    copy_count = 0;
    move_count = 0;
}

void MatrixV1::printStats() {
    std::cout << "MatrixV1 Statistics:\n"
              << "  Instance Count: " << instance_count << "\n"
              << "  Copy Count:     " << copy_count << "\n"
              << "  Move Count:     " << move_count << "\n";
}

int MatrixV1::getInstanceCount() { return instance_count; }
int MatrixV1::getCopyCount() { return copy_count; }
int MatrixV1::getMoveCount() { return move_count; }

// Stream Output
std::ostream& operator<<(std::ostream& os, const MatrixV1& mat) {
    os << "MatrixV1 " << mat.rows() << "x" << mat.cols() << ":\n";
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
