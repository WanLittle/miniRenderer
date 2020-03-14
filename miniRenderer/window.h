#pragma once
#include <iostream>
#include "SDL.h"

class Window
{
public:
    Window(int height, int width);
    void setPixelColor(int x, int y, int r, int g, int b, int a);
    void update();
    void delay();
    void destroy();
    ~Window();
public:
    SDL_Window* mWin;
    SDL_Surface* mCanvas;
};