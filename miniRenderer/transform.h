#pragma once

#include "vec.h"
#include "mat.h"

extern Matrix ModelView;
extern Matrix Projection;
extern Matrix Viewport;

// 指定深度范围
void glDepthRange(float n, float f);
// 视口变换矩阵
void glViewport(int x, int y, int w, int h);

// View 矩阵
void glLookat(Vec3f eye, Vec3f center, Vec3f up);

// 透视投影矩阵
void glPerspective(float fovy, float aspect, float zNear, float zFar);
void glOrtho(float left, float right, float bottom, float top, float near, float far);
