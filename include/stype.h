#ifndef SLAB_STYPE_H
#define SLAB_STYPE_H
#define GLM_FORCE_SWIZZLE
#include <glm.hpp>
#include <ext.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#ifdef SLAB_PRECISION_DOUBLE
typedef double SFloat;
typedef glm::dvec2 SVector2;
typedef glm::dvec3 SVector3;
typedef glm::dvec4 SVector4;
typedef glm::dmat4 SMatrix4;
typedef glm::dmat3 SMatrix3;
typedef glm::fvec4 SColor4;
typedef glm::fvec3 SColor3;
#define S_CONST_FLOAT(num) num
#else
typedef float SFloat;
typedef glm::fvec2 SVector2;
typedef glm::fvec3 SVector3;
typedef glm::fvec4 SVector4;
typedef glm::fmat4 SMatrix4;
typedef glm::fmat3 SMatrix3;
typedef glm::fvec3 SColor3;
typedef glm::fvec4 SColor4;
#define S_CONST_FLOAT(num) num##f
#endif

typedef glm::ivec2 SCoord;
typedef glm::ivec2 SVector2Int;
typedef glm::ivec3 SVector3Int;
typedef glm::ivec4 SVector4Int;
typedef glm::uvec4 SNormColor4;
typedef glm::uvec3 SNormColor3;

typedef unsigned char SByte;
typedef uint64_t SIndex;

struct SVertex{
    SVector3 position;
    SVector3 normal;
    SColor4 color;
};


struct SFragment{
    SVector2Int coord;
    SVector3 normal;
    SColor4 color;
    SFloat depth;
};


namespace SConst {
    const SVector3 up(0, 1, 0);
    const SVector3 right(1, 0, 0);
    const SVector3 forward(0, 0, 1);


    const SMatrix4 identity4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    );

    const SMatrix3 identity3(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
    );
}

#define MIN(X, Y)\
    ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y)\
    ((X) > (Y) ? (X) : (Y))


#define NORMALIZE_COLOR_3(COLOR)\
    (SNormColor3(\
        MAX(0, MIN(255, (SByte)((COLOR).r * 255))),\
        MAX(0, MIN(255, (SByte)((COLOR).g * 255))),\
        MAX(0, MIN(255, (SByte)((COLOR).b * 255)))\
    ))
#define NORMALIZE_COLOR_4(COLOR) \
    (SNormColor4(\
        MAX(0, MIN(255, (SByte)((COLOR).r * 255))),\
        MAX(0, MIN(255, (SByte)((COLOR).g * 255))),\
        MAX(0, MIN(255, (SByte)((COLOR).b * 255))),\
        MAX(0, MIN(255, (SByte)((COLOR).a * 255)))\
    ))

#endif //SLAB_STYPE_H
