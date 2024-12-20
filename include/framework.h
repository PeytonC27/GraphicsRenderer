#pragma once

#include "SDL2/SDL.h"
#include "vector2.h"
#include "rgba.h"

class Framework {
public:
    Framework(int height, int width);
    ~Framework();

    void drawLine(Vector2 origin, Vector2 destination, RGBA color);
    void drawLine(Vector2 origin, Vector2 direction, float distance);
	void drawCircle(Vector2 center, float radius);
    int saveImage();
    void clear();
    void render();

private:
    int height;
    int width;
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL; 
};