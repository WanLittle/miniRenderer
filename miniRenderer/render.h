#pragma once
#include "vec.h"
#include "mat.h"
#include "tgaimage.h"

class Window;
struct IShader;

// Bresenham 直线算法
void line(Vec2i v1, Vec2i v2, std::shared_ptr<Window>& win, int r, int g, int b, int a);

// Line sweeping 扫描线算法
void triangle_linesweeping(mat<4, 3, float>& clipc, IShader& shader, std::shared_ptr<Window>& win, float* zbuffer);
// 重心坐标系 光栅化算法
void triangle_barycentric(mat<4, 3, float>& pts, IShader& shader, std::shared_ptr<Window>& win, float* zbuffer);

struct IShader {
	virtual ~IShader();
	// 顶点着色器
	virtual Vec4f vertex(int iface, int nthvert) = 0;
	// 像素着色器
	virtual bool fragment(Vec3f bar, TGAColor& color) = 0;
};
