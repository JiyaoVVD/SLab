//
// Created by Jiyao on 2024/11/2.
//

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

inline int64_t icross2(const SVector2Int& v0, const SVector2Int& v1)
{
    return (int64_t)v0.x * v1.y - (int64_t)v0.y * v1.x;
}

#endif //MATH_UTILS_H

