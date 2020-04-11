#include <algorithm>

#include "render.h"
#include "window.h"
#include "transform.h"

IShader::~IShader()
{
}

void line(Vec2i v1, Vec2i v2, std::shared_ptr<Window>& win, int r, int g, int b, int a)
{
	bool steep = std::abs(v1.x - v2.x) < std::abs(v1.y - v2.y);
	if (steep)
	{
		std::swap(v1.x, v1.y);
		std::swap(v2.x, v2.y);
	}
	if (v1.x > v2.x)
	{
		std::swap(v1.x, v2.x);
		std::swap(v1.y, v2.y);
	}
	int deltaX = v2.x - v1.x;
	int deltaY = std::abs(v2.y - v1.y);
	int error = deltaX / 2;
	int yStep;
	int y = v1.y;
	yStep = v1.y < v2.y ? 1 : -1;
	for (int x = v1.x; x <= v2.x; ++x)
	{
		if (steep)
			win->setPixelColor(y, x, r, g, b, a);
		else
			win->setPixelColor(x, y, r, g, b, a);
		error = error - deltaY;
		if (error < 0)
		{
			y = y + yStep;
			error = error + deltaX;
		}
	}
}

// TODO: 暂时没有正确插值
void triangle_linesweeping(mat<4, 3, float>& clipc, IShader& shader, std::shared_ptr<Window>& win, float* zbuffer)
{
	mat<3, 4, float> vpc = (Viewport * clipc).transpose(); // 结果坐标转置成行优先存储
	mat<3, 2, float> vpc2;
	for (int i = 0; i < 3; i++) vpc2[i] = proj<2>(vpc[i] / vpc[i][3]);

	if (vpc2[0].y == vpc2[1].y && vpc2[0].y == vpc2[2].y) // 忽略退化的三角形
		return;
	if (vpc2[0].y > vpc2[1].y) std::swap(vpc2[0], vpc2[1]);
	if (vpc2[0].y > vpc2[2].y) std::swap(vpc2[0], vpc2[2]);
	if (vpc2[1].y > vpc2[2].y) std::swap(vpc2[1], vpc2[2]);

	int total_height = vpc2[2].y - vpc2[0].y;
	for (int i = 0; i < total_height; i++)
	{
		bool second_half = i > vpc2[1].y - vpc2[0].y || vpc2[1].y == vpc2[0].y;
		int segment_height = second_half ? vpc2[2].y - vpc2[1].y : vpc2[1].y - vpc2[0].y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? vpc2[1].y - vpc2[0].y : 0)) / segment_height;
		Vec2f A = vpc2[0] + (vpc2[2] - vpc2[0]) * alpha;
		Vec2f B = second_half ? vpc2[1] + (vpc2[2] - vpc2[1]) * beta : vpc2[0] + (vpc2[1] - vpc2[0]) * beta;
		if (A.x > B.x)
			std::swap(A, B);

		TGAColor color;
		int l = (int)A.x, r = (int)B.x;
		for (int j = l; j <= r; j++)
		{
			float t = (float)(j - l) / (r - l);
			int wPos = j;
			int yPos = vpc2[0].y + i;

			// 属性插值
			Vec3f bc_clip = Vec3f(0.33f, 0.33f, 0.33f);
			float frag_depth = clipc[2] * bc_clip;

			bool discard = shader.fragment(bc_clip, color);
			if (!discard)
			{
				if (zbuffer[wPos + yPos * win->getWidth()] < frag_depth)
				{
					zbuffer[wPos + yPos * win->getWidth()] = frag_depth;
					win->setPixelColor(wPos, yPos, color.bgra[2], color.bgra[1], color.bgra[0], color.bgra[3]);
				}
			}
		}
	}
}

Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P)
{
	Vec3f s[2];
	for (int i = 2; i--; )
	{
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}
	Vec3f u = cross(s[0], s[1]);
    // TODO：为什么这种情况是退化三角形
	if (std::abs(u[2]) > 1e-2) //  u[2] is integer. 如果为0 则三角形ABC是退化的degenerate
	{
		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	}
	return Vec3f(-1, 1, 1); // 丢弃退化三角形
}

void triangle_barycentric(const mat<4, 3, float>& clipc, const mat<4, 3, float>& ndc, 
    IShader& shader, std::shared_ptr<Window>& win, float* zbuffer)
{
	mat<3, 4, float> screen_triangle_point = (Viewport * ndc).transpose(); // 转置 方便获取每一个点
	mat<3, 2, float> screen_triangle_point_2;
	for (int i = 0; i < 3; i++)
        screen_triangle_point_2[i] = proj<2>(screen_triangle_point[i]);

	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f clamp(win->getWidth() - 1, win->getHeight() - 1);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			bboxmin[j] = std::max(0.f, std::min(bboxmin[j], screen_triangle_point_2[i][j]));
			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], screen_triangle_point_2[i][j]));
		}
	}

	Vec2i P;
	TGAColor color;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
	{
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
		{
			Vec3f bc_screen = barycentric(screen_triangle_point_2[0], screen_triangle_point_2[1], screen_triangle_point_2[2], P);

			// 属性插值
			Vec3f bc_clip = Vec3f(bc_screen.x / clipc[3][0], bc_screen.y / clipc[3][1], bc_screen.z / clipc[3][2]);
			bc_clip = bc_clip / (bc_clip.x + bc_clip.y + bc_clip.z);
            // 根据质心坐标插值出深度
			float frag_depth = clipc[2] * bc_clip;
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0 || zbuffer[P.x + P.y * win->getWidth()] > frag_depth) // 深度测试
				continue;
			bool discard = shader.fragment(bc_clip, color);
			if (!discard)
			{
				zbuffer[P.x + P.y * win->getWidth()] = frag_depth;
				win->setPixelColor(P.x, P.y, color.bgra[2], color.bgra[1], color.bgra[0], color.bgra[3]);
			}
		}
	}
}
