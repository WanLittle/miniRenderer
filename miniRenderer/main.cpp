#include <iostream>
#include <algorithm>
#include <memory>

#include "model.h"
#include "window.h"
#include "geometry.h"

const int width = 800;
const int height = 800;

void line(Vec2i v1, Vec2i v2, std::shared_ptr<Window>& win, Uint32 color);
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, std::shared_ptr<Window>& win, Uint32 color);

int main(int argc, char* argv[])
{
    std::shared_ptr<Window> win(new Window(width, height));
    std::shared_ptr<Model> model(new Model("../assets/african_head.obj"));

    Uint32 white = SDL_MapRGB(win->mCanvas->format, 255, 255, 255);
    Uint32 red = SDL_MapRGB(win->mCanvas->format, 255, 0, 0);

    for (int i = 0; i < model->nfaces(); i++) 
    {
        std::vector<int> face = model->face(i);
        Vec3f v0 = model->vert(face[0]);
        Vec3f v1 = model->vert(face[1]);
        Vec3f v2 = model->vert(face[2]);
        int x0 = (v0.x + 1.0) * width / 2.0;
        int y0 = (v0.y + 1.0) * height / 2.0;
        int x1 = (v1.x + 1.0) * width / 2.0;
        int y1 = (v1.y + 1.0) * height / 2.0;
        int x2 = (v2.x + 1.0) * width / 2.0;
        int y2 = (v2.y + 1.0) * height / 2.0;
        triangle(Vec2i(x0, y0), Vec2i(x1, y1), Vec2i(x2, y2), win, white);
    }

    win->update();
    win->delay();
    win->destroy();

    return 0;
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
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, std::shared_ptr<Window>& win, Uint32 color)
{
    if (t0.y == t1.y && t0.y == t2.y) // 忽略退化的三角形
        return;

    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);

    int total_height = t2.y - t0.y;
    for (int i = 0; i < total_height; i++) 
    {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
        if (A.x > B.x) 
            std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) 
        {
            win->setPixelColor(j, t0.y + i, color);
        }
    }
}