//
// Created by tangrui01 on 2024/10/18.
//

#ifndef SLAB_CAMERA_H
#define SLAB_CAMERA_H

#include "stype.h"

class Camera {
public:
    Camera();

private:
    SFloat aspect;
    SFloat fov;

    SMatrix4 projMatrix;
    SMatrix4 viewMatrix;
    SMatrix4 viewportMatrix;
};


#endif //SLAB_CAMERA_H
