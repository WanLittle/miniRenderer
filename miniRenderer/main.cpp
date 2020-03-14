#include <iostream>
#include <algorithm>
#include <memory>
#include <cmath>


#include "model.h"
#include "window.h"
#include "geometry.h"


#define PI std::acos(-1)

const int width = 600;
const int height = 800;

// 视口变换矩阵
Matrix viewport(int x, int y, int w, int h, float n = 0.0, float f = 1.0) 
{
    Matrix m = Matrix::identity(4);
    m[3][0] = x + w / 2.0f;
    m[3][1] = y + h / 2.0f;
    m[3][2] = (n + f) / 2.0f;
    //m[3][3] = 1.0f;

    m[0][0] = w / 2.0f;
    m[1][1] = h / 2.0f;
    m[2][2] = (f - n) / 2.f;
    return m;
}

// View 矩阵
Matrix lookat(Vec3f eye, Vec3f center, Vec3f up) 
{
    Vec3f const z((center - eye).normalize()); // z axis
    Vec3f const x((z ^ up).normalize()); // x axis
    Vec3f const y(x ^ z); // y axis

    Matrix result;
    result[0][0] = x.x;
    result[1][0] = x.y;
    result[2][0] = x.z;
    result[0][1] = y.x;
    result[1][1] = y.y;
    result[2][1] = y.z;
    result[0][2] = -z.x;
    result[1][2] = -z.y;
    result[2][2] = -z.z;
    result[3][0] = -(x * eye);
    result[3][1] = -(y * eye);
    result[3][2] = (z * eye);
    result[3][3] = 1.0f;
    return result;
}

// 透视投影矩阵
Matrix perspective(float fovy, float aspect, float zNear, float zFar)
{
    Matrix res;

    float const tanHalfFovy = tan(fovy / static_cast<float> (2));

    res[0][0] = static_cast<float>(1) / (aspect * tanHalfFovy);
    res[1][1] = static_cast<float>(1) / (tanHalfFovy);
    res[2][3] = -static_cast<float>(1);

    res[2][2] = -(zFar + zNear) / (zFar - zNear);
    res[3][2] = -(static_cast<float>(2)* zFar* zNear) / (zFar - zNear);

    return res;
}

Vec3f world2screen(Vec3f v);
void line(Vec2i v1, Vec2i v2, std::shared_ptr<Window>& win, Uint32 color);
void triangle(Vec3f* pts, float* zbuffer, std::shared_ptr<Window>& win, float* intensity);

int main(int argc, char* argv[])
{
    std::shared_ptr<Window> win(new Window(width, height));
    std::shared_ptr<Model> model(new Model("../assets/african_head.obj"));

    Uint32 white = SDL_MapRGB(win->mCanvas->format, 255, 255, 255);
    Uint32 red = SDL_MapRGB(win->mCanvas->format, 255, 0, 0);

    float* zbuffer = new float[width * height];
    for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());


    {   // 绘制模型
        Vec3f light_dir = Vec3f(0, 0, 3).normalize();
        Vec3f eye(0, 0, -3);
        Vec3f center(0, 0, 0);
        Matrix Model = Matrix::identity(4);
        Matrix View = lookat(eye, center, Vec3f(0, 1, 0));
        Matrix Projection = perspective(45.0 / 180.0 * PI, static_cast<float>(width) / height, 1.0f, 100.0f);
        Matrix ViewPort = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);

        //std::cerr << Model << std::endl;
        //std::cerr << View << std::endl;
        //std::cerr << Projection << std::endl;
        //std::cerr << ViewPort << std::endl;
        //Matrix z = (Model * View * Projection * ViewPort);
        //std::cerr << z << std::endl;

        for (int i = 0; i < model->nfaces(); i++)
        {
            std::vector<int> face = model->face(i);
            Vec3f screen_coords[3];
            Vec3f world_coords[3];
            float intensity[3];
            for (int j = 0; j < 3; j++) 
            {
                Vec3f v = model->vert(face[j]);
                Matrix clip = Matrix(v);
                clip = clip * Model;
                clip = clip * View;
                clip = clip * Projection;
                clip[0][0] = clip[0][0] / clip[0][3]; //透视除法
                clip[0][1] = clip[0][1] / clip[0][3];
                clip[0][2] = clip[0][2] / clip[0][3];
                clip[0][3] = 1.0;
                screen_coords[j] = Vec3f(clip * ViewPort);
                world_coords[j] = v;
                intensity[j] = model->norm(i, j) * light_dir;
            }
            triangle(screen_coords, zbuffer, win, intensity);
        }
    }

    win->update();
    win->delay();
    win->destroy();

    return 0;
}

Vec3f world2screen(Vec3f v)
{
    return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
}

// Bresenham 直线算法
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
        if(steep)
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

// Line sweeping 扫描线算法
void triangle(Vec3f* pts, float* zbuffer, std::shared_ptr<Window>& win, float* intensity)
{
    if (pts[0].y == pts[1].y && pts[0].y == pts[2].y) // 忽略退化的三角形
        return;

    if (pts[0].y > pts[1].y) std::swap(pts[0], pts[1]);
    if (pts[0].y > pts[2].y) std::swap(pts[0], pts[2]);
    if (pts[1].y > pts[2].y) std::swap(pts[1], pts[2]);

    int total_height = pts[2].y - pts[0].y;
    for (int i = 0; i < total_height; i++) 
    {
        bool second_half = i > pts[1].y - pts[0].y || pts[1].y == pts[0].y;
        int segment_height = second_half ? pts[2].y - pts[1].y : pts[1].y - pts[0].y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? pts[1].y - pts[0].y : 0)) / segment_height;
        Vec3f A = pts[0] + (pts[2] - pts[0]) * alpha;
        float aIntensity = intensity[0] + (intensity[2] - intensity[0]) * alpha;
        Vec3f B = second_half ? pts[1] + (pts[2] - pts[1]) * beta : pts[0] + (pts[1] - pts[0]) * beta;
        float bIntensity = second_half ? intensity[1] + (intensity[2] - intensity[1]) * beta : 
            intensity[0] + (intensity[1] - intensity[0]) * beta;
        if (A.x > B.x) 
            std::swap(A, B);
        int l = (int) A.x, r = (int) B.x;
        for (int j = l; j <= r; j++)
        {
            float t = (float)(j - l) / (r - l);
            float z = A.z * (1 - t) + B.z * t;
            float pIntensity = aIntensity * (1 - t) + bIntensity * t;
            int wPos = j;
            int yPos = pts[0].y + i;
            if (zbuffer[wPos + yPos * width] < z)
            {
                zbuffer[wPos + yPos * width] = z;
                win->setPixelColor(wPos, yPos, pIntensity * 255, pIntensity * 255, pIntensity * 255, 255);
            }
        }
    }
}