#include "transform.h"

Matrix ModelView;
Matrix Projection;
Matrix Viewport;
float n = 0.0, f = 1.0;

void glDepthRange(float _n, float _f)
{
    n = _n;
    f = _f;
}

// 视口变换
void glViewport(int x, int y, int w, int h)
{
	Viewport = Matrix::identity();
	Viewport[0][3] = x + w / 2.0f;
	Viewport[1][3] = y + h / 2.0f;
	Viewport[2][3] = (n + f) / 2.0f;
	//Viewport[3][3] = 1.0f;

	Viewport[0][0] = w / 2.0f;
	Viewport[1][1] = h / 2.0f;
	Viewport[2][2] = (f - n) / 2.f;
}

// 视图变换
void glLookat(Vec3f eye, Vec3f center, Vec3f up)
{
	Vec3f const z_axis((eye - center).normalize()); // z 轴
	Vec3f const x_axis(cross(up, z_axis).normalize()); // x 轴
	Vec3f const y_axis(cross(z_axis, x_axis)); // y 轴

	ModelView[0][0] = x_axis.x;
	ModelView[0][1] = x_axis.y;
	ModelView[0][2] = x_axis.z;
    ModelView[0][3] = 0.0f;

	ModelView[1][0] = y_axis.x;
	ModelView[1][1] = y_axis.y;
	ModelView[1][2] = y_axis.z;
    ModelView[2][3] = 0.0f;

	ModelView[2][0] = z_axis.x;
	ModelView[2][1] = z_axis.y;
	ModelView[2][2] = z_axis.z;
    ModelView[2][3] = 0.0f;

	ModelView[0][3] = -(x_axis * eye);
	ModelView[1][3] = -(y_axis * eye);
	ModelView[2][3] = -(z_axis * eye);
	ModelView[3][3] = 1.0f;
}

// 透视投影变换
void glPerspective(float fovy, float aspect, float zNear, float zFar)
{
	float const tanHalfFovy = tan(fovy / static_cast<float> (2));

	Projection[0][0] = static_cast<float>(1) / (aspect * tanHalfFovy);
	Projection[1][1] = static_cast<float>(1) / (tanHalfFovy);
	Projection[2][3] = -static_cast<float>(1);

	Projection[2][2] = -(zFar + zNear) / (zFar - zNear);
	Projection[3][2] = -(static_cast<float>(2)* zFar* zNear) / (zFar - zNear);
}

// 正交投影变换
void glOrtho(float left, float right, float bottom, float top, float near, float far)
{
    float x_range = right - left;
    float y_range = top - bottom;
    float z_range = far - near;

    Projection[0][0] = 2 / x_range;
    Projection[1][1] = 2 / y_range;
    Projection[2][2] = -2 / z_range;
    Projection[0][3] = -(left + right) / x_range;
    Projection[1][3] = -(bottom + top) / y_range;
    Projection[2][3] = -(near + far) / z_range;
    Projection[3][3] = 1.0f;
}