//
// Created by tangrui01 on 2024/10/18.
//

#ifndef SLAB_CAMERA_H
#define SLAB_CAMERA_H

#include "stype.h"
#include "math_utils.h"

class Camera {
public:
    enum CameraMode{
        ORTHODOX,
        PERSPECTIVE,
    };
public:
    Camera(SVector3 pos, SVector3 lookPos, SVector3 up);
    const SMatrix4 & projection() const;
    const SMatrix4 & view() const;
    SVector3 position() const;
    SVector3 direction() const;
    SVector3 up() const;
    void setPosition(const SVector3& pos);
    void setPosition(SFloat x, SFloat y, SFloat z);
    void setDirection(const SVector3& dir, const SVector3& up);
    void setDirection(SFloat x, SFloat y, SFloat z, SFloat upX, SFloat upY, SFloat upZ);
    void setLookat(const SVector3& lookat, const SVector3& worldUp);
    void setLookat(SFloat x, SFloat y, SFloat z, SFloat upX, SFloat upY, SFloat upZ);
    void setOrthodox(SFloat left, SFloat right, SFloat top, SFloat bottom, SFloat near, SFloat far);
    void setPerspective(SFloat fov, SFloat aspect, SFloat near, SFloat far);
private:
    void calcViewMatrix(const SVector3& position, const SVector3& dir, const SVector3& up);
private:
    CameraMode mode;

    SVector3 pos;
    SVector3 dir;
    SVector3 worldUp;

    SMatrix4 projMatrix;
    SMatrix4 viewMatrix;
    SMatrix4 viewportMatrix;

    SMatrix4 orthoProjection(SFloat left, SFloat right, SFloat top, SFloat bottom, SFloat near, SFloat far);
    SMatrix4 persProjection(SFloat fov, SFloat aspect, SFloat near, SFloat far);
};


#endif //SLAB_CAMERA_H
