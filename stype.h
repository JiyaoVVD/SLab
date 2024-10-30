//
// Created by tangrui01 on 2024/10/18.
//

#ifndef SLAB_STYPE_H
#define SLAB_STYPE_H

#include <graphics.h>
#include <glm.hpp>

#ifdef SLAB_PRECISION_DOUBLE
typedef double SType;
typedef glm::dvec2 SVector2;
typedef glm::dvec3 SVector3;
typedef glm::dvec4 SVector4;
typedef glm::fvec4 SColor4;
typedef glm::fvec3 SColor3;
#define S_CONST_FLOAT(num) num
#else
typedef float SType;
typedef glm::fvec2 SVector2;
typedef glm::fvec3 SVector3;
typedef glm::fvec4 SVector4;
typedef glm::fvec3 SColor3;
typedef glm::fvec4 SColor4;
#define S_CONST_FLOAT(num) num##f
#endif

typedef glm::ivec2 SCoord;
typedef glm::ivec2 SVector2Int;
typedef glm::ivec3 SVector3Int;
typedef glm::ivec4 SVector4Int;
typedef glm::ivec4 SNormColor4;
typedef glm::ivec3 SNormColor3;

typedef COLORREF HEX_COLOR;
#define RGB_TO_HEX_COLOR(R, G, B) COLORREF(((B) << 16) | ((G) << 8) | (R))

#endif //SLAB_STYPE_H