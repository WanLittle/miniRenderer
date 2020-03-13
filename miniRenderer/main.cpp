#include <iostream>
#include <algorithm>

#include "model.h"
#include "SDL.h"

const int width = 800;
const int height = 800;

void line(int x0, int y0, int x1, int y1, SDL_Surface* screen, Uint32 color);
Uint32 getpixel(SDL_Surface* surface, int x, int y); // 获取颜色
void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel, bool flipY = true); // 填充颜色

int main(int argc, char* argv[])
{
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 初始化窗口
    SDL_Window* win = SDL_CreateWindow("mini renderer", 100, 100, width, height, SDL_WINDOW_SHOWN);
    
    if (win == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    
    SDL_Surface* screen = SDL_GetWindowSurface(win);

    Uint32 white = SDL_MapRGB(screen->format, 255, 255, 255);
    Uint32 red = SDL_MapRGB(screen->format, 255, 0, 0);

    Model* model = new Model("../assets/african_head.obj");

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
            line(x0, y0, x1, y1, screen, white);
        }
    }

    SDL_UpdateWindowSurface(win);
    SDL_Delay(10000);

    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

// Bresenham's line algorithm
void line(int x0, int y0, int x1, int y1, SDL_Surface* screen, Uint32 color)
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
            putpixel(screen, y, x, color);
        else 
            putpixel(screen, x, y, color);
        error = error - deltaY;
        if (error < 0)
        {
            y = y + yStep;
            error = error + deltaX;
        }
    }
}

Uint32 getpixel(SDL_Surface* surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16*)p;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32*)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel, bool flipY)
{
    if (flipY) y = surface->h - y;
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    x = std::max(0, std::min(surface->w - 1, x));
    y = std::max(0, std::min(surface->h - 1, y));
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
    
    switch (bpp) 
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16*)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) 
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else 
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32*)p = pixel;
        break;
    }
}