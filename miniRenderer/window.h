#pragma once
#include <iostream>
#include "SDL.h"

class Window
{
public:
	Window(int height, int width);
	~Window();

	void setPixelColor(int x, int y, int r, int g, int b, int a);
	void update();
	void delay();
	void destroy();

	int getWidth() { return mWidth; }
	int getHeight() { return mHeight; }
	SDL_Surface* getCanvas() { return mCanvas; }
	SDL_Window* getWindow() { return mWin; }

private:
	SDL_Surface* mCanvas;
	SDL_Window* mWin;
	int mWidth;
	int mHeight;
};
