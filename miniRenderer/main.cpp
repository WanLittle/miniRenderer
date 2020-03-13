#include <iostream>
#include <algorithm>
#include <memory>

#include "model.h"
#include "window.h"

const int width = 800;
const int height = 800;

void line(int x0, int y0, int x1, int y1, std::shared_ptr<Window>& win, Uint32 color);

int main(int argc, char* argv[])
{
    std::shared_ptr<Window> win(new Window(width, height));
    std::shared_ptr<Model> model(new Model("../assets/african_head.obj"));

    Uint32 white = SDL_MapRGB(win->mCanvas->format, 255, 255, 255);
    Uint32 red = SDL_MapRGB(win->mCanvas->format, 255, 0, 0);

    for (int i = 0; i < model->nfaces(); i++) 
    {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++) 
        {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            int x0 = (v0.x + 1.0) * width / 2.0;
            int y0 = (v0.y + 1.0) * height / 2.0;
            int x1 = (v1.x + 1.0) * width / 2.0;
            int y1 = (v1.y + 1.0) * height / 2.0;
            line(x0, y0, x1, y1, win, white);
        }
    }

    win->update();
    win->delay();
    win->destroy();

    return 0;
}

// Bresenham's line algorithm
void line(int x0, int y0, int x1, int y1, std::shared_ptr<Window>& win, Uint32 color)
{
    bool steep = std::abs(x0 - x1) < std::abs(y0 - y1);
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int deltaX = x1 - x0;
    int deltaY = std::abs(y1 - y0);
    int error = deltaX / 2;
    int yStep;
    int y = y0;
    yStep = y0 < y1 ? 1 : -1;
    for (int x = x0; x <= x1; ++x) 
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