#pragma once

#include "vec.h"
#include "mat.h"

extern Matrix ModelView;
extern Matrix Projection;
extern Matrix Viewport;

Vec3f world2screen(Vec3f v, int width, int height);

// 视口变换矩阵
void viewport(int x, int y, int w, int h, float n, float f);

void viewport(int x, int y, int w, int h);

// View 矩阵
void lookat(Vec3f eye, Vec3f center, Vec3f up);

// 透视投影矩阵
void perspective(float fovy, float aspect, float zNear, float zFar);
void projection(float coeff);
