#include "window.h"
#include <algorithm>

Uint32 getpixel(SDL_Surface* surface, int x, int y); // 获取颜色
void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel, bool flipY = true); // 填充颜色

Window::Window(int height, int width)
{
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // 初始化窗口
    mWin = SDL_CreateWindow("mini renderer", 100, 100, width, height, SDL_WINDOW_SHOWN);

    if (mWin == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    mCanvas = SDL_GetWindowSurface(mWin);
}

void Window::setPixelColor(int x, int y, int r, int g, int b, int a)
{
    putpixel(mCanvas, x, y, SDL_MapRGBA(mCanvas->format, r, g, b, a), true);
}

void Window::update()
{
    SDL_UpdateWindowSurface(mWin);
}

void Window::delay()
{
    SDL_Delay(10000);
}

void Window::destroy()
{
    SDL_DestroyWindow(mWin);
}

Window::~Window()
{
    SDL_Quit();
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
    x = std::max(0, std::min(surface->w - 1, x));
    y = std::max(0, std::min(surface->h - 1, y));

    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
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