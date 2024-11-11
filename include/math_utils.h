//
// Created by Jiyao on 2024/11/2.
//

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <iostream>
#include <iomanip>
#include <glm.hpp>
#include "stype.h"


inline int64_t icross2(const SVector2Int& v0, const SVector2Int& v1)
{
    return (int64_t)v0.x * v1.y - (int64_t)v0.y * v1.x;
}


template<int D1, int D2>
inline void PrintMatrix(const glm::mat<D1, D2, SFloat>& mat){
    for(int i = 0; i < D1; ++i){
        for(int j = 0; j < D2; ++j){
            std::cout << std::setprecision(2) << mat[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}


template<int D>
inline void PrintVector(const glm::vec<D, SFloat>& vec){
    std::cout << "( ";
    for(int i = 0; i < D - 1; ++i){
        std::cout << vec[i] << ", ";
    }
    std::cout << vec[D - 1] << " )" << std::endl;
}


#endif //MATH_UTILS_H

