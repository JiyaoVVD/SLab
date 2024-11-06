//
// Created by tangrui01 on 2024/10/18.
//

#ifndef SLAB_CAMERA_H
#define SLAB_CAMERA_H

#include "stype.h"

class Camera {
public:
    Camera(SVector3 pos, SVector3 lookPos, SVector3 up);
    const SMatrix4& projection();
    const SMatrix4& view();
    SVector3 position();
    SVector3 direction();
    SVector3 up();

public:
    enum CameraMode{
        ORTHODOX,
        PERSPECTIVE,
    };

private:

    CameraMode mode;

    // perspective
    SFloat aspect;
    SFloat fov;

    SMatrix4 projMatrix;
    SMatrix4 viewMatrix;
    SMatrix4 viewportMatrix;

    SMatrix4 orthoProjection(SFloat left, SFloat right, SFloat top, SFloat bottom, SFloat near, SFloat far);
    SMatrix4 persProjection(SFloat fov, SFloat aspect, SFloat near, SFloat far);
};


#endif //SLAB_CAMERA_H
