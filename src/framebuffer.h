//
// Created by Jiyao on 2024/11/2.
//
#pragma once
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <algorithm>
#include "stype.h"
#include "math_utils.h"

struct FrameBuffer{
    unsigned width;
    unsigned height;
    unsigned char *buffer;
    SFloat* depthBuffer;
};

FrameBuffer* CreateFrameBuffer(const unsigned width, const unsigned height);
void ReleaseFrameBuffer(FrameBuffer* frameBuffer);
void RenderFrameBuffer(const FrameBuffer* frameBuffer);
void ClearFrameBuffer(FrameBuffer* frameBuffer, const SNormColor3& color);
inline void DrawPixel(FrameBuffer *frameBuffer, const SVector2Int& p1, const SNormColor3& color);
void DrawLine(FrameBuffer *frameBuffer, const SVector2Int& p0, const SVector2Int& p1, const SNormColor3& c0, const SNormColor3& c1);
void DrawTriangle(
        FrameBuffer *frameBuffer,
        const SVector2Int& p0,
        const SVector2Int& p1,
        const SVector2Int& p2,
        const SNormColor3& c0,
        const SNormColor3& c1,
        const SNormColor3& c2);
#endif //FRAMEBUFFER_H
