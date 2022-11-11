#pragma once
//NumericType
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include "VectorMethods.h"

using std::string;
using std::vector;

template <typename T>
string matrixToString(vector<vector<T>> matrix) {
    string str = "";

    for (auto& vec : matrix) {
        str += vectorToString(vec);
    }

    return "[\n" + str + "]\n\n";
}


//Rotations & shapy stuff
template <typename T>
void transpose(vector<vector<T>>* matrix) {
    vector<vector<T>> transposed(matrix->at(0).size(), vector<T>(matrix->size(), NULL));

    for (size_t i = 0; i < matrix->size(); i++)
    {
        for (size_t j = 0; j < matrix->at(0).size(); j++)
        {
            transposed[j][i] = matrix->at(i).at(j);
        }
    }

    *matrix = transposed;
}

template <typename T>
vector<vector<T>> transpose(vector<vector<T>> matrix) {
    transpose(&matrix);
    return matrix;
}

template <typename T>
void flipHor(vector<vector<T>>* matrix) {
    for (auto& vec : *matrix) {
        reverse(&vec);
    }
}

template <typename T>
vector<vector<T>> flipHor(vector<vector<T>> matrix) {
    flipHor(&matrix);
    return matrix;
}

template <typename T>
void flipVer(vector<vector<T>>* matrix) {
    reverse(matrix);
}

template <typename T>
vector<vector<T>> flipVer(vector<vector<T>> matrix) {
    flipVer(&matrix);
    return matrix;
}

template <typename T>
void rotate180(vector<vector<T>>* matrix) {
    flipHor(matrix);
    flipVer(matrix);
}

template <typename T>
vector<vector<T>> rotate180(vector<vector<T>> matrix) {
    rotate180(&matrix);
    return matrix;
}

template <typename T>
void rotate90CCW(vector<vector<T>>* matrix) {
    transpose(matrix);
    flipVer(matrix);
}

template <typename T>
vector<vector<T>> rotate90CCW(vector<vector<T>> matrix) {
    rotate90CCW(&matrix);
    return matrix;
}

template <typename T>
void rotate90CW(vector<vector<T>>* matrix) {
    flipVer(matrix);
    transpose(matrix);
}

template <typename T>
vector<vector<T>> rotate90CW(vector<vector<T>> matrix) {
    rotate90CW(&matrix);
    return matrix;
}

template <typename T>
bool isRectangular(vector<vector<T>> matrix) {
    if (!matrix.size()) {
        return true;
    }
    size_t size = matrix[0].size();
    for (auto& vec : matrix) {
        if (vec.size() != size) {
            return false;
        }
    }
    return true;
}


//Matrix-vector conversions
template <typename T>
vector<T> columnToVector(vector<vector<T>> matrix, int col) {
    vector<T> column;
    for (auto& vec : matrix) {
        column.push_back(vec[col]);
    }
    return column;
}

template <typename T>
void vectorToColumn(vector<vector<T>>* matrix, vector<T> column, int col) {
    for (size_t i = 0; i < matrix->size(); i++) {
        matrix->at(i).at(col) = column[i];
    }
}

template <typename T>
vector<vector<T>> vectorToColumn(vector<vector<T>> matrix, vector<T> column, int col) {
    vectorToColumn(&matrix);
    return matrix;
}

template <typename T>
vector<T> matrixToVector(vector<vector<T>> matrix) {
    vector<T> vecMatrix;
    for (auto& vec : matrix)
    {
        vecMatrix.insert(vecMatrix.end(), vec.begin(), vec.end());
    }
    return vecMatrix;
}

template <typename T>
vector<vector<T>> vectorToMatrix(vector<T> vec, int rows, int cols) {
    vector<vector<T>> matrix(rows);
    for (size_t i = 0; i < rows; i++)
    {
        std::copy(vec.begin() + i * cols,
            vec.begin() + i * cols + rows + 1,
            std::back_inserter(matrix[i]));
    }
    return matrix;
}

//Sorting
template <typename T>
void sortRowsByColumn(vector<vector<T>>* matrix, int col) {
    std::sort(matrix->begin(), matrix->end(),
        [col](const vector<T>& lhs, const vector<T> rhs) {
            return lhs[col] < rhs[col];
        });
}

template <typename T>
vector<vector<T>> sortRowsByColumn(vector<vector<T>> matrix, int col) {
    sortRowsByColumn(&matrix, col);
    return matrix;
}

template <typename T>
void sortColumnsByRow(vector<vector<T>>* matrix, int row) {
    transpose(matrix);
    sortRowsByColumn(matrix, row);
    transpose(matrix);
}

template <typename T>
vector<vector<T>> sortColumnsByRow(vector<vector<T>> matrix, int row) {
    sortColumnsByRow(&matrix, row);
    return matrix;
}

template <typename T>
void sortColumn(vector<vector<T>>* matrix,  int col) {
    auto column = columnToVector(*matrix, col);
    sort(&column);
    vectorToColumn(matrix, column, col);
}

template <typename T>
vector<vector<T>> sortColumn(vector<vector<T>> matrix, int col) {
    sortColumn(&matrix, col);
    return matrix;
}

template <typename T>
void sortRow(vector<vector<T>>* matrix, int row) {
    sort(&(matrix->at(row)));
}

template <typename T>
vector<vector<T>> sortRow(vector<vector<T>> matrix, int row) {
    sortRow(&matrix, row);
    return matrix;
}

template <typename T>
void sortAllRows(vector<vector<T>>* matrix) {
    for (auto& vec : *matrix)
    {
        sort(&vec);
    }
}

template <typename T>
vector<vector<T>> sortAllRows(vector<vector<T>> matrix) {
    sortAllRows(&matrix);
    return matrix;
}

template <typename T>
void sortAllColumns(vector<vector<T>>* matrix) {
    transpose(matrix);
    sortAllRows(matrix);
    transpose(matrix);
}

template <typename T>
vector<vector<T>> sortAllColumns(vector<vector<T>> matrix) {
    sortAllColumns(&matrix);
    return matrix;
}

template <typename T>
void completeIndividualSort(vector<vector<T>>* matrix) {
    sortAllRows(matrix);
    sortAllColumns(matrix);
}

template <typename T>
vector<vector<T>> completeIndividualSort(vector<vector<T>> matrix) {
    completeIndividualSort(&matrix);
    return matrix;
}


template <typename T>
void completeHolisticSort(vector<vector<T>>* matrix) {
    auto vec = matrixToVector(*matrix);
    sort(&vec);
    *matrix = vectorToMatrix(vec, matrix->size(), matrix->at(0).size());
}

template <typename T>
vector<vector<T>> completeHolisticSort(vector<vector<T>> matrix) {
    completeHolisticSort(&matrix);
    return matrix;
}


//Shuffling 
template <typename T>
void shuffleColumn(vector<vector<T>>* matrix, int col) {
    auto column = columnToVector(*matrix, col);
    shuffle(&column);
    vectorToColumn(matrix, column, col);
}

template <typename T>
vector<vector<T>> shuffleColumn(vector<vector<T>> matrix, int col) {
    shuffleColumn(&matrix, col);
    return matrix;
}

template <typename T>
void shuffleRow(vector<vector<T>>* matrix, int row) {
    shuffle(&(matrix->at(row)));
}

template <typename T>
vector<vector<T>> shuffleRow(vector<vector<T>> matrix, int row) {
    shuffleRow(&matrix, row);
    return matrix;
}

template <typename T>
void shuffleAllRows(vector<vector<T>>* matrix) {
    for (auto& vec : *matrix)
    {
        shuffle(&vec);
    }
}

template <typename T>
vector<vector<T>> shuffleAllRows(vector<vector<T>> matrix) {
    shuffleAllRows(&matrix);
    return matrix;
}

template <typename T>
void shuffleAllColumns(vector<vector<T>>* matrix) {
    transpose(matrix);
    shuffleAllRows(matrix);
    transpose(matrix);
}

template <typename T>
vector<vector<T>> shuffleAllColumns(vector<vector<T>> matrix) {
    shuffleAllColumns(&matrix);
    return matrix;
}

template <typename T>
void completeShuffle(vector<vector<T>>* matrix) {
    shuffleAllRows(matrix);
    shuffleAllColumns(matrix);
}

template <typename T>
vector<vector<T>> completeShuffle(vector<vector<T>> matrix) {
    completeShuffle(&matrix);
    return matrix;
}



//other

template <typename T>
void removeColumn(vector<vector<T>>* matrix, int col) {
    for (size_t i = 0; i < matrix->size(); i++)
    {
        matrix->at(i).erase(matrix->at(i).begin() + col);
    }
}

template <typename NumericType>
vector<NumericType> medianByColumn(vector<vector<NumericType>>* matrix, int col) {
    sortRowsByColumn(matrix);
    return matrix->at((matrix->size() - 1) / 2);
}

template <typename NumericType>
vector<NumericType> medianByColumn(vector<vector<NumericType>> matrix, int col) {
    sortRowsByColumn(&matrix, col);
    return matrix[(matrix.size() -1) / 2];
}





template <typename NumericType>
NumericType matrixMinimum(vector<vector<NumericType>> matrix) {
    auto min = std::numeric_limits<NumericType>::max();
    NumericType temp;
    for (auto& vec : matrix)
    {
        temp = min(vec);
        if (temp < min) {
            min = temp;
        }
    }
    return min;
}

template <typename NumericType>
NumericType matrixMaximum(vector<NumericType> vec) {
    auto max = std::numeric_limits<NumericType>::min();
    for (auto& i : vec)
    {
        if (i < max) {
            max = i;
        }
    }
    return max;
}


