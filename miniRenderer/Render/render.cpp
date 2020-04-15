#include <algorithm>

#include "render.h"
#include "../Math/transform.h"
#include "pipeline.h"


void line(glm::vec2i v1, glm::vec2i v2, FrameBuffer* framebuffer, int r, int g, int b, int a)
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
			framebuffer->drawPixel(y, x, glm::vec4f(r, g, b, a));
		else
            framebuffer->drawPixel(x, y, glm::vec4f(r, g, b, a));
		error = error - deltaY;
		if (error < 0)
		{
			y = y + yStep;
			error = error + deltaX;
		}
	}
}

//// TODO: 暂时没有正确插值
//void triangle_linesweeping(mat<4, 3, float>& clipc, BaseShader& shader, FrameBuffer* framebuffer)
//{
//	mat<3, 4, float> vpc = (Viewport * clipc).transpose(); // 结果坐标转置成行优先存储
//	mat<3, 2, float> vpc2;
//	for (int i = 0; i < 3; i++) vpc2[i] = proj<2>(vpc[i] / vpc[i][3]);
//
//	if (vpc2[0].y == vpc2[1].y && vpc2[0].y == vpc2[2].y) // 忽略退化的三角形
//		return;
//	if (vpc2[0].y > vpc2[1].y) std::swap(vpc2[0], vpc2[1]);
//	if (vpc2[0].y > vpc2[2].y) std::swap(vpc2[0], vpc2[2]);
//	if (vpc2[1].y > vpc2[2].y) std::swap(vpc2[1], vpc2[2]);
//
//	int total_height = vpc2[2].y - vpc2[0].y;
//	for (int i = 0; i < total_height; i++)
//	{
//		bool second_half = i > vpc2[1].y - vpc2[0].y || vpc2[1].y == vpc2[0].y;
//		int segment_height = second_half ? vpc2[2].y - vpc2[1].y : vpc2[1].y - vpc2[0].y;
//		float alpha = (float)i / total_height;
//		float beta = (float)(i - (second_half ? vpc2[1].y - vpc2[0].y : 0)) / segment_height;
//		Vec2f A = vpc2[0] + (vpc2[2] - vpc2[0]) * alpha;
//		Vec2f B = second_half ? vpc2[1] + (vpc2[2] - vpc2[1]) * beta : vpc2[0] + (vpc2[1] - vpc2[0]) * beta;
//		if (A.x > B.x)
//			std::swap(A, B);
//
//		TGAColor color;
//		int l = (int)A.x, r = (int)B.x;
//		for (int j = l; j <= r; j++)
//		{
//			float t = (float)(j - l) / (r - l);
//			int wPos = j;
//			int yPos = vpc2[0].y + i;
//
//			// 属性插值
//			Vec3f bc_clip = Vec3f(0.33f, 0.33f, 0.33f);
//			float frag_depth = clipc[2] * bc_clip;
//
//			bool discard = shader.fragment(bc_clip, color);
//			if (!discard)
//			{
//				if (zbuffer[wPos + yPos * win->getWidth()] < frag_depth)
//				{
//					zbuffer[wPos + yPos * win->getWidth()] = frag_depth;
//					win->setPixelColor(wPos, yPos, color.bgra[2], color.bgra[1], color.bgra[0], color.bgra[3]);
//				}
//			}
//		}
//	}
//}
