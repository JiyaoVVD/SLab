//
// Created by tangrui01 on 2024/10/18.
//

#include "camera.h"

Camera::Camera(SVector3 pos, SVector3 lookPos, SVector3 worldUp){
    SMatrix4 trans = SConst::identity4;
    for(int i = 0; i < 3; i++){
        trans[i][3] = -pos[i];
    }
    SVector3 front = glm::normalize((lookPos - pos));

    SVector3 right = glm::normalize(glm::cross(front, worldUp));
    SVector3 up = glm::normalize(glm::cross(right, front));
    SMatrix4 rot = SConst::identity4;
    for(int i = 0; i < 3; i++){
        rot[0][i] = right[i];
        rot[1][i] = up[i];
        rot[2][i] = -front[i];
    }

    viewMatrix = rot * trans;
}


SVector3
Camera::position(){
    return SVector3{
            viewMatrix[0][3],
            viewMatrix[1][3],
            viewMatrix[2][3]
    };
}


SVector3
Camera::up(){
    return viewMatrix[1];
}


SVector3
Camera::direction(){
    return -viewMatrix[2];
}

const SMatrix4&
Camera::view(){
    return viewMatrix;
}

const SMatrix4&
Camera::projection(){
    return projMatrix;
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
    SFloat near_view = -near;
    SFloat far_view = -far;
    SFloat theta = glm::radians(fov / S_CONST_FLOAT(2.0));
    SFloat top = near * glm::tan(theta);
    SFloat bottom = -top;
    SFloat right = top * aspect;
    SFloat left = -right;

    SMatrix4 orth = orthoProjection(left, right, top, bottom, near, far);
    SMatrix4 per2orth{0};
    per2orth[0][0] = near_view;
    per2orth[1][1] = near_view;
    per2orth[2][2] = near_view + far_view;
    per2orth[2][3] = -near_view * far_view;
    per2orth[3][2] = 1.0f;

    return orth * per2orth;
}
