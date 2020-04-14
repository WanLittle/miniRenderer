#pragma once

#include "../Math/vec.h"
#include "../Math/mat.h"
#include "../Shader/baseshader.h"

class FrameBuffer;

// Bresenham 直线算法
void line(glm::Vec2i v1, glm::Vec2i v2, FrameBuffer* framebuffer, int r, int g, int b, int a);

// Line sweeping 扫描线算法
//void triangle_linesweeping(mat<4, 3, float>& clipc, BaseShader& shader, FrameBuffer* framebuffer);