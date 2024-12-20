#include "../include/framework.h"
#include "SDL2/SDL_image.h"
#include <cmath>

struct Line {
    Vector2 origin;
    Vector2 destination;
};

struct RGB {
	int r;
	int g;
	int b;
};

RGB getRGB(float ratio) {
	
	int rgbIndex = (int)std::roundf(ratio * 767);
	
	if (rgbIndex <= 255) {
		return RGB { 255 - rgbIndex, rgbIndex, 0 }; 
	}
	else if (rgbIndex <= 511) {
		return RGB { 0, 511 - rgbIndex, rgbIndex % 256 };
	}
	else {
		return RGB { rgbIndex % 256 , 0, 767 - rgbIndex }; 
	}
	
	// 765 colors
	// 255 0 0
	// 0 255 0
	// 0 0 255
}

Framework::Framework(int height, int width) : height(height), width(width) {
    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL as video
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);   // setting draw color
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderClear(renderer);                      // clear new window
    SDL_RenderPresent(renderer);                    // reflects changes done in window
}

Framework::~Framework() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// This now uses the Bresenham Line Algorithm
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void Framework::drawLine(Vector2 origin, Vector2 destination, RGBA color) {

    // adjust coordinates for the image
    origin = Vector2(origin.x, -(origin.y - height));
    destination = Vector2(destination.x, -(destination.y - height));
	
	int x1 = (int)std::roundf(origin.x);
	int x2 = (int)std::roundf(destination.x);
	int y1 = (int)std::roundf(origin.y);
	int y2 = (int)std::roundf(destination.y);
	
	int dx = std::abs(x2 - x1);
	int sx = (x1 < x2) ? 1 : -1;
	int dy = -std::abs(y2 - y1);
	int sy = (y1 < y2) ? 1 : -1;
	int err = dx + dy;
	
	float alpha = 100;

	
	while (true) {
		// draw pixel
		//RGB color = getRGB((float)x1 / width);
		
		//SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
		SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, 50);
        SDL_RenderDrawPoint(renderer, x1, y1);
		
		//std::cout << "Plotted: " << Vector2(x1, y1) << std::endl;
		
		// brensen line alg
		if ((x1 == x2) && (y1 == y2)) break;
		float e2 = err << 1;
		if (e2 >= dy) {
			if (x1 == x2) break;
			err += dy;
			x1 += sx;
		}
		if (e2 <= dx) {
			if (y1 == y2) break;
			err += dx;
			y1 += sy;
		}
	}	

}


void Framework::drawLine(Vector2 origin, Vector2 direction, float distance) {

    drawLine(origin, origin + direction.normalized() * distance, RGBA(255, 255, 255, 255));

}

void Framework::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}

void Framework::render() {
    SDL_RenderPresent(renderer);                    // reflects changes done in window
}

int Framework::saveImage() {// Create an SDL_Surface to store the renderer's pixel data
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
        0, width, height, 32, SDL_PIXELFORMAT_RGBA32);

    if (!surface) {
        std::cerr << "Failed to create surface: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Read pixel data from the renderer into the surface
    if (SDL_RenderReadPixels(renderer, nullptr, surface->format->format, surface->pixels, surface->pitch) != 0) {
        std::cerr << "Failed to read pixels: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return -1;
    }

    // Save the surface to a PNG file
    int result = IMG_SavePNG(surface, "a.png");
    if (result != 0) {
        std::cerr << "Failed to save image: " << IMG_GetError() << std::endl;
    }

    // Clean up
    SDL_FreeSurface(surface);
    return result;
}