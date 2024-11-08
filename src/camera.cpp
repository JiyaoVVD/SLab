//
// Created by tangrui01 on 2024/10/18.
//

#include "camera.h"


SMatrix4
Camera::calcRotationMat(const SVector3& direction, const SVector3& worldUp) {
    auto normDir = glm::normalize(direction);
    SVector3 right = glm::normalize(glm::cross(normDir, worldUp));
    SVector3 up = glm::normalize(glm::cross(right, normDir));
    SMatrix4 rot = SConst::identity4;
    for(int i = 0; i < 3; i++){
        rot[0][i] = right[i];
        rot[1][i] = up[i];
        rot[2][i] = normDir[i];
    }
    return rot;
}


Camera::Camera(SVector3 pos, SVector3 lookPos, SVector3 worldUp){
    SMatrix4 trans = SConst::identity4;
    for(int i = 0; i < 3; i++){
        trans[i][3] = -pos[i];
    }
    SVector3 front = glm::normalize((lookPos - pos));
    auto rot = calcRotationMat(front, worldUp);

    viewMatrix = rot * trans;
}


inline SVector3
Camera::position() const{
    return SVector3{
            -viewMatrix[0][3],
            -viewMatrix[1][3],
            -viewMatrix[2][3]
    };
}


inline SVector3
Camera::up() const{
    return viewMatrix[1];
}


inline SVector3
Camera::direction() const{
    return -viewMatrix[2];
}


void
Camera::setPosition(const SVector3& pos) {
    viewMatrix[0][3] = pos.x;
    viewMatrix[1][3] = pos.y;
    viewMatrix[2][3] = pos.z;

}


void
Camera::setPosition(SFloat x, SFloat y, SFloat z) {
    viewMatrix[0][3] = x;
    viewMatrix[1][3] = y;
    viewMatrix[2][3] = z;
}


void
Camera::setDirection(const SVector3& dir, const SVector3& worldUp) {
    auto rot = calcRotationMat(dir, worldUp);
    for(int i = 0; i < 3; i++){
        viewMatrix[0][i] = rot[0][i];
        viewMatrix[1][i] = rot[1][i];
        viewMatrix[2][i] = rot[2][i];
    }
}


void
Camera::setDirection(SFloat x, SFloat y, SFloat z,SFloat upX, SFloat upY, SFloat upZ) {
    auto rot = calcRotationMat(SVector3(x, y, z), SVector3(upX, upY, upZ));
    for(int i = 0; i < 3; i++){
        viewMatrix[0][i] = rot[0][i];
        viewMatrix[1][i] = rot[1][i];
        viewMatrix[2][i] = rot[2][i];
    }
}


void
Camera::setLookat(const SVector3 &lookat, const SVector3& up) {
    setDirection(lookat - position(), up);
}


void
Camera::setLookat(SFloat x, SFloat y, SFloat z, SFloat upX, SFloat upY, SFloat upZ) {
    setDirection(SVector3(x, y, z) - position(), SVector3(upX, upY, upZ));
}


const SMatrix4 &
Camera::view() const {
    return viewMatrix;
}

const SMatrix4&
Camera::projection() const {
    return projMatrix;
}


void
Camera::setPerspective(SFloat fov, SFloat aspect, SFloat near, SFloat far) {
    projMatrix = persProjection(fov, aspect, near, far);
}


void
Camera::setOrthodox(SFloat left, SFloat right, SFloat top, SFloat bottom, SFloat near, SFloat far) {
    projMatrix = orthoProjection(left, right, top, bottom, near, far);
}


SMatrix4
Camera::orthoProjection(SFloat left, SFloat right, SFloat top, SFloat bottom, SFloat near, SFloat far){
    near = -near;
    far = -far;
    SMatrix4 translate = SConst::identity4;
    translate[0][3] = -((left + right) / S_CONST_FLOAT(2.0));
    translate[1][3] = -((top + bottom) / S_CONST_FLOAT(2.0));
    translate[2][3] = -((near + far) / S_CONST_FLOAT(2.0));

    SMatrix4 scale = SConst::identity4;
    scale[0][0] = S_CONST_FLOAT(2.0) / (right - left);
    scale[1][1] = S_CONST_FLOAT(2.0) / (top - bottom);
    scale[2][2] = S_CONST_FLOAT(2.0) / (near - far);

    return scale * translate;
}


SMatrix4
Camera::persProjection(SFloat fov, SFloat aspect, SFloat near, SFloat far){
    SFloat rad = glm::radians(fov);
    SFloat tanHalfFov = glm::tan(rad / S_CONST_FLOAT(2.0));

    SFloat f = S_CONST_FLOAT(1.0) / tanHalfFov;
    SFloat range = far - near;
    SFloat A = -(far + near) / range;
    SFloat B = S_CONST_FLOAT(-2.0) * far * near / range;

    SMatrix4 projectionMatrix{
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, A, B,
        0, 0, 1, 1
    };
    PrintMatrix(projectionMatrix);
    return projectionMatrix;
}
