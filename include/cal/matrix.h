#ifndef CAL_MATRIX_H
#define CAL_MATRIX_H

#include <vector>
#include <stdexcept>
#include <iostream>

template <typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;
    size_t rows;
    size_t cols;

public:
    // 默认构造函数
    Matrix() : rows(0), cols(0) {}

    // 指定大小的构造函数
    Matrix(size_t r, size_t c) : rows(r), cols(c) {
        data.resize(rows, std::vector<T>(cols, T()));
    }

    // 使用初始数据的构造函数
    Matrix(const std::vector<std::vector<T>>& initialData) {
        if (initialData.empty()) {
            rows = 0;
            cols = 0;
            return;
        }
        
        rows = initialData.size();
        cols = initialData[0].size();
        data = initialData;
    }

    // 获取行数
    size_t getRows() const { return rows; }

    // 获取列数
    size_t getCols() const { return cols; }

    // 访问元素
    T& at(size_t r, size_t c) {
        if (r >= rows || c >= cols) {
            throw std::out_of_range("Matrix indices out of range");
        }
        return data[r][c];
    }

    const T& at(size_t r, size_t c) const {
        if (r >= rows || c >= cols) {
            throw std::out_of_range("Matrix indices out of range");
        }
        return data[r][c];
    }

    // 重载[]操作符用于访问行
    std::vector<T>& operator[](size_t r) {
        if (r >= rows) {
            throw std::out_of_range("Row index out of range");
        }
        return data[r];
    }

    const std::vector<T>& operator[](size_t r) const {
        if (r >= rows) {
            throw std::out_of_range("Row index out of range");
        }
        return data[r];
    }

    // 重载加法运算符
    Matrix<T> operator+(const Matrix<T>& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions must match for addition");
        }

        Matrix<T> result(rows, cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result[i][j] = data[i][j] + other[i][j];
            }
        }
        return result;
    }

    // 重载乘法运算符（矩阵乘法）
    Matrix<T> operator*(const Matrix<T>& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Matrix dimensions invalid for multiplication");
        }

        Matrix<T> result(rows, other.cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < other.cols; j++) {
                result[i][j] = T();  // 初始化为0
                for (size_t k = 0; k < cols; k++) {
                    result[i][j] += data[i][k] * other[k][j];
                }
            }
        }
        return result;
    }

    // 重载乘法运算符（标量乘法）
    Matrix<T> operator*(const T& scalar) const {
        Matrix<T> result(rows, cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }

    // 输出矩阵内容
    friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
        for (size_t i = 0; i < m.rows; i++) {
            os << "[ ";
            for (size_t j = 0; j < m.cols; j++) {
                os << m[i][j];
                if (j < m.cols - 1) os << ", ";
            }
            os << " ]" << std::endl;
        }
        return os;
    }
};

// 标量右乘
template <typename T>
Matrix<T> operator*(const T& scalar, const Matrix<T>& mat) {
    return mat * scalar;
}

#endif // CAL_MATRIX_H

/*
使用实例
Matrix<int> A({
        {1, 2, 3},
        {4, 5, 6}
}); 

Matrix<int> B({
    {7, 8},
    {9, 10},
    {11, 12}
});
    
// 矩阵乘法
Matrix<int> C = A * B;


*/