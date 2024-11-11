//
// Created by tangrui01 on 2024/10/18.
//

#include "camera.h"


void
Camera::calcViewMatrix(const SVector3& position, const SVector3& direction, const SVector3& worldUp) {
    SMatrix4 trans = SConst::identity4;
    trans[3] = SVector4(-position, 1);
    auto normDir = glm::normalize(direction);
    SVector3 right = glm::normalize(glm::cross(worldUp, direction));
    SVector3 up = glm::normalize(glm::cross(normDir, right));
    SMatrix4 rot = SConst::identity4;
    for(int i = 0; i < 3; i++){
        rot[i][0] = right[i];
        rot[i][1] = up[i];
        rot[i][2] = normDir[i];
    }
    viewMatrix = rot * trans;
}


Camera::Camera(SVector3 pos, SVector3 lookPos, SVector3 worldUp):
    pos(pos), dir(glm::normalize(lookPos - pos)), worldUp(worldUp)
{
    calcViewMatrix(pos, dir, worldUp);
}



SVector3
Camera::position() const{
    return pos;
}

SVector3
Camera::direction() const{
    return dir;
}


SVector3
Camera::up() const{
    return worldUp;
}


void
Camera::setPosition(const SVector3& pos) {
    this->pos = pos;
    calcViewMatrix(pos, dir, worldUp);
}


void
Camera::setPosition(SFloat x, SFloat y, SFloat z) {
    this->pos = {x, y, z};
    calcViewMatrix(pos, dir, worldUp);
}


void
Camera::setDirection(const SVector3& dir, const SVector3& worldUp) {
    this->dir = glm::normalize(dir);
    this->worldUp = glm::normalize(worldUp);
    calcViewMatrix(pos, dir, worldUp);
}


void
Camera::setDirection(SFloat x, SFloat y, SFloat z,SFloat upX, SFloat upY, SFloat upZ) {
    this->dir = glm::normalize(SVector3(x, y, z));
    this->worldUp = glm::normalize(SVector3(upX, upY, upZ));
    calcViewMatrix(pos, dir, worldUp);
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
        0, 0, -A, 1,
        0, 0, B, 0
    };
    return projectionMatrix;
}
