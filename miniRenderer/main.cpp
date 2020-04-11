#include <iostream>
#include <algorithm>
#include <memory>
#include <cmath>
#include <string>
using namespace std;

#include "model.h"
#include "window.h"
#include "vec.h"
#include "mat.h"
#include "transform.h"
#include "render.h"

#define PI std::acos(-1)

const int width = 600;
const int height = 800;

std::shared_ptr<Window> win(new Window(width, height));
std::shared_ptr<Model> model;
std::vector<string> model_paths = {
    //"../assets/african_head/african_head.obj",
    "../assets/floor/floor.obj",
    //"../assets/diablo3_pose/diablo3_pose.obj",
};

Vec3f light_dir(0, 0, 3);
Vec3f       eye(0, -0.5, -2.0);
Vec3f    center(0, 0, 0);
Vec3f        up(0, 1, 0);

struct Shader : public IShader
{
	mat<2, 3, float> varying_uv;  // 顶点的UV坐标
	mat<4, 3, float> varying_clip_tri; // 裁剪空间的三角形三角形坐标
	mat<3, 3, float> varying_nrm; // 顶点的法线
	mat<4, 3, float> ndc_tri;     // NDC 三角形顶点的坐标

	virtual Vec4f vertex(int iface, int nthvert) override
	{
		// UV 坐标
		varying_uv.set_col(nthvert, model->uv(iface, nthvert));

		// 法线
		varying_nrm.set_col(nthvert, proj<3>((Projection * ModelView).invert_transpose() * embed<4>(model->normal(iface, nthvert), 0.f)));

		// 坐标变换 -> 裁剪空间
		Vec4f gl_Vertex = Projection * ModelView * embed<4>(model->vert(iface, nthvert));

		// 设置裁剪空间的顶点坐标
        varying_clip_tri.set_col(nthvert, gl_Vertex);

		// 透视除法
		ndc_tri.set_col(nthvert, gl_Vertex / gl_Vertex[3]);

		return gl_Vertex;
	}

	virtual bool fragment(Vec3f bar, TGAColor& color) override
	{
		Vec3f bn = (varying_nrm * bar).normalize();
		Vec2f uv = varying_uv * bar;

		mat<3, 3, float> A;
		A[0] = proj<3>(ndc_tri.col(1) - ndc_tri.col(0));
		A[1] = proj<3>(ndc_tri.col(2) - ndc_tri.col(0));
		A[2] = bn;

		mat<3, 3, float> AI = A.invert();

		Vec3f i = AI * Vec3f(varying_uv[0][1] - varying_uv[0][0], varying_uv[0][2] - varying_uv[0][0], 0);
		Vec3f j = AI * Vec3f(varying_uv[1][1] - varying_uv[1][0], varying_uv[1][2] - varying_uv[1][0], 0);

		mat<3, 3, float> B;
		B.set_col(0, i.normalize());
		B.set_col(1, j.normalize());
		B.set_col(2, bn);

		Vec3f n = (B * model->normal(uv)).normalize();

		float diff = std::max(0.f, n * light_dir);
		color = model->diffuse(uv) * diff;

		return false;
	}
};


int main(int argc, char* argv[])
{
	float* zbuffer = new float[width * height];
	for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

	glLookat(eye, center, up);
    glDepthRange(0.0f, 1.0f);
	glViewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	glPerspective(90.0 / 180.0 * PI, static_cast<float>(width) / height, 0.5f, 100.0f);
	light_dir = proj<3>((Projection * ModelView * embed<4>(light_dir, 0.0f))).normalize();

	Shader shader;
    for (int m = 0; m < model_paths.size(); ++m)
    {
        model = std::make_shared<Model>(model_paths[m].c_str());
        for (int i = 0; i < model->nfaces(); ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                shader.vertex(i, j);
            }
            triangle_barycentric(shader.varying_clip_tri, shader.ndc_tri, shader, win, zbuffer);
        }
    }

	win->update();
	win->delay();
	win->destroy();

	return 0;
}
