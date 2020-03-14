#include "transform.h"

Matrix ModelView;
Matrix Projection;
Matrix Viewport;

Vec3f world2screen(Vec3f v, int width, int height)
{
	return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
}

void viewport(int x, int y, int w, int h) {
	Viewport = Matrix::identity();
	Viewport[0][3] = x + w / 2.f;
	Viewport[1][3] = y + h / 2.f;
	Viewport[2][3] = 1.f;
	Viewport[0][0] = w / 2.f;
	Viewport[1][1] = h / 2.f;
	Viewport[2][2] = 0;
}

void projection(float coeff) {
	Projection = Matrix::identity();
	Projection[3][2] = coeff;
}

//void lookat(Vec3f eye, Vec3f center, Vec3f up)
//{
//	Vec3f z = (eye - center).normalize();
//	Vec3f x = cross(up, z).normalize();
//	Vec3f y = cross(z, x).normalize();
//	Matrix Minv = Matrix::identity();
//	Matrix Tr = Matrix::identity();
//	for (int i = 0; i < 3; i++) {
//		Minv[0][i] = x[i];
//		Minv[1][i] = y[i];
//		Minv[2][i] = z[i];
//		Tr[i][3] = -center[i];
//	}
//	ModelView = Minv * Tr;
//}

void viewport(int x, int y, int w, int h, float n, float f)
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

void lookat(Vec3f eye, Vec3f center, Vec3f up)
{
	Vec3f const z((center - eye).normalize()); // z axis
	Vec3f const x(cross(z, up).normalize()); // x axis
	Vec3f const y(cross(x, z)); // y axis

	ModelView[0][0] = x.x;
	ModelView[0][1] = x.y;
	ModelView[0][2] = x.z;
	ModelView[1][0] = y.x;
	ModelView[1][1] = y.y;
	ModelView[1][2] = y.z;
	ModelView[2][0] = -z.x;
	ModelView[2][1] = -z.y;
	ModelView[2][2] = -z.z;
	ModelView[0][3] = -(x * eye);
	ModelView[1][3] = -(y * eye);
	ModelView[2][3] = (z * eye);
	ModelView[3][3] = 1.0f;
}

void perspective(float fovy, float aspect, float zNear, float zFar)
{
	float const tanHalfFovy = tan(fovy / static_cast<float> (2));

	Projection[0][0] = static_cast<float>(1) / (aspect * tanHalfFovy);
	Projection[1][1] = static_cast<float>(1) / (tanHalfFovy);
	Projection[3][2] = -static_cast<float>(1);

	Projection[2][2] = -(zFar + zNear) / (zFar - zNear);
	Projection[2][3] = -(static_cast<float>(2)* zFar* zNear) / (zFar - zNear);
}
