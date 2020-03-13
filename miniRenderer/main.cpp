#include <iostream>
#include <algorithm>
#include <memory>

#include "model.h"
#include "window.h"
#include "geometry.h"

const int width = 800;
const int height = 800;

Vec3f world2screen(Vec3f v);
void line(Vec2i v1, Vec2i v2, std::shared_ptr<Window>& win, Uint32 color);
void triangle(Vec3f* pts, float* zbuffer, std::shared_ptr<Window>& win, Uint32 color);

int main(int argc, char* argv[])
{
    std::shared_ptr<Window> win(new Window(width, height));
    std::shared_ptr<Model> model(new Model("../assets/african_head.obj"));

    Uint32 white = SDL_MapRGB(win->mCanvas->format, 255, 255, 255);
    Uint32 red = SDL_MapRGB(win->mCanvas->format, 255, 0, 0);

    std::cout << std::numeric_limits<double>::max() << std::endl;
    std::cout << std::numeric_limits<double>::min() << std::endl;
    std::cout << std::numeric_limits<float>::max() << std::endl;
    std::cout << std::numeric_limits<float>::min() << std::endl;
    std::cout << std::numeric_limits<int>::max() << std::endl;
    std::cout << std::numeric_limits<int>::min() << std::endl;

    float* zbuffer = new float[width * height];
    for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

    Vec3f light_dir(0, 0, -1);

    for (int i = 0; i < model->nfaces(); i++) 
    {
        std::vector<int> face = model->face(i);
        Vec3f pts[3];
        for (int i = 0; i < 3; i++) pts[i] = world2screen(model->vert(face[i]));
        triangle(pts, zbuffer, win, SDL_MapRGB(win->mCanvas->format, rand() % 255, rand() % 255, rand() % 255));
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
void line(Vec2i v1, Vec2i v2, std::shared_ptr<Window>& win, Uint32 color)
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
            win->setPixelColor(y, x, color);
        else
            win->setPixelColor(x, y, color);
        error = error - deltaY;
        if (error < 0)
        {
            y = y + yStep;
            error = error + deltaX;
        }
    }
}

// Line sweeping 扫描线算法
void triangle(Vec3f* pts, float* zbuffer, std::shared_ptr<Window>& win, Uint32 color)
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
        Vec3f B = second_half ? pts[1] + (pts[2] - pts[1]) * beta : pts[0] + (pts[1] - pts[0]) * beta;
        if (A.x > B.x) 
            std::swap(A, B);
        int l = (int) A.x, r = (int) B.x;
        for (int j = l; j <= r; j++)
        {
            float t = (float)(j - l) / (r - l);
            float z = A.z * (1 - t) + B.z * t;
            int wPos = j, yPos = pts[0].y + i;
            if (zbuffer[wPos + yPos * width] < z)
            {
                zbuffer[wPos + yPos * width] = z;
                win->setPixelColor(wPos, yPos, color);
            }
        }
    }
}