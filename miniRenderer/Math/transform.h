#pragma once

#include "vec.h"
#include "mat.h"

namespace glm
{

    // 视口变换
    glm::Matrix viewport(int x, int y, int w, int h, float n, float f);

    // 视图变换
    glm::Matrix lookat(glm::vec3f eye, glm::vec3f center, glm::vec3f up);

    // 透视投影矩阵
    glm::Matrix perspective(float fovy, float aspect, float zNear, float zFar);

    // 正交投影变换
    glm::Matrix ortho(float left, float right, float bottom, float top, float near, float far);

    float radians(float degrees);
    float degrees(float radians);

} // namespace glmath