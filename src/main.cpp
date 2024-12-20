#include "../include/vector2.h"
#include "../include/framework.h"
#include "../include/rgba.h"
#include "SDL2/SDL.h"
#include <iostream>
#include <cmath>
#include <list>
#include <random>
#include <set>
#include <ctime>
#include <algorithm>


void draw_slime(Framework& fw, int iterations, int width, int height) {
    std::list<Vector2> points;

    Vector2 origin(width/2, height/2);
    points.push_back(origin);

    std::set<Vector2> existing;

    std::srand(std::time(nullptr));

    for (int i = 1; i <= iterations && points.size() > 0; i++) {
        Vector2 current = points.front();
        points.pop_front();

        if (current.x < 0 || current.x > width)
            continue;
        if (current.y < 0 || current.y > height)
            continue;

        existing.insert(current);


        if (std::rand() % 2 == 0 && !existing.count(current + Vector2::up * 10)) {
            fw.drawLine(current, Vector2::up, 10);
            points.push_back(current + Vector2::up * 10);
        }
        if (std::rand() % 2 == 0 && !existing.count(current + Vector2::down * 10)) {
            fw.drawLine(current, Vector2::down, 10);
            points.push_back(current + Vector2::down * 10);
        }
        if (std::rand() % 2 == 0 && !existing.count(current + Vector2::left * 10)) {
            fw.drawLine(current, Vector2::left, 10);
            points.push_back(current + Vector2::left * 10);
        }
        if (std::rand() % 2 == 0 && !existing.count(current + Vector2::right * 10)) {
            fw.drawLine(current, Vector2::right, 10);
            points.push_back(current + Vector2::right * 10);
        }
    }
}

void draw_coral(Framework& fw, int iterations, int width, int height) {

    struct CoralPoint {
        Vector2 point;
        float angle;
        int it;
    };

    float pi = 3.1415926535f;

    std::list<CoralPoint> points;

    Vector2 origin(0, 0);
    points.push_back(CoralPoint { origin, -90 }); // -90

    std::set<Vector2> existing;

    std::srand(std::time(nullptr));
    std::random_device rd;
    std::mt19937 gen(rd());

    int coralLeafLen = 300;

    for (int i = 1; i <= iterations && points.size() > 0; i++) {
        if (points.size() == 0)
            break;

        CoralPoint current = points.front();
        points.pop_front();

        if (current.point.x < 0 || current.point.x > width)
            continue;
        if (current.point.y < 0 || current.point.y > height)
            continue;
        if (current.angle > 990)
            continue;

        existing.insert(current.point);

        int newConnections = std::rand() % 4; // 0, 1, 2, or 3


        for (int j = 0; j < newConnections; j++) {
            float newAngle = current.angle + (std::rand() % 15);
			coralLeafLen = std::rand() % 400 + 100;

            Vector2 destinationVector = Vector2(std::roundf(current.point.x + coralLeafLen * std::cos(newAngle * pi / 180)),
                                                std::roundf(current.point.y - coralLeafLen * std::sin(newAngle * pi / 180)));

            Vector2 adjustedDest = Vector2(destinationVector.x - (int)destinationVector.x % 5,
                                           destinationVector.y - (int)destinationVector.y % 5);


            if (existing.count(adjustedDest))
                continue;

            fw.drawLine(current.point, destinationVector, RGBA(255, 255, 255, 255));
            points.push_back(CoralPoint { destinationVector, newAngle } );
            existing.insert(adjustedDest);
        }
    }
}

void draw_black_hole(Framework& fw, int iterations, int width, int height) {
	struct HorizonPoint {
        Vector2 point;
        float angle;
    };

    float pi = 3.1415926535f;

    std::list<HorizonPoint> points;

    Vector2 origin(width/2, height/2 + 1000);
	Vector2 center(width/2, height/2);
	float innerRadius = 750;
	float outerRadius = 1000;
	
    points.push_back(HorizonPoint { origin, 0 }); // -90

    std::set<Vector2> existing;

    std::srand(std::time(nullptr));
    std::random_device rd;
    std::mt19937 gen(rd());

    int horizonLineLen = 300;
	
	auto gradientFunction = RGBA::generateGradientFunction(RGBA(255, 255, 255), RGBA(255, 127, 0));

    for (int i = 1; i <= iterations && points.size() > 0; i++) {
        if (points.size() == 0)
            break;

        HorizonPoint current = points.front();
        points.pop_front();

        if (current.point.x < 0 || current.point.x > width)
            continue;
        if (current.point.y < 0 || current.point.y > height)
            continue;
        if (current.angle > 990)
            continue;

        existing.insert(current.point);

        int newConnections = std::rand() % 4; // 0, 1, 2, or 3


        for (int j = 0; j < newConnections; j++) {
            float newAngle = current.angle + (std::rand() % 30);
			horizonLineLen = std::rand() % 400 + 100;

            Vector2 destinationVector = Vector2(std::roundf(current.point.x + horizonLineLen * std::cos(newAngle * pi / 180)),
                                                std::roundf(current.point.y - horizonLineLen * std::sin(newAngle * pi / 180)));
												
			// if this new point is in the black circle, stop
			if ((std::pow(center.y - destinationVector.y, 2) + std::pow(center.x - destinationVector.x, 2)) <= std::pow(innerRadius, 2))
				continue;
			
			// if the new point is outside the expected range, there's a chance it'll break
			if ((std::pow(center.y - destinationVector.y, 2) + std::pow(center.x - destinationVector.x, 2)) >= std::pow(outerRadius, 2)) {
				float dist = (destinationVector - center).magnitude();
				float oddsToStop = 1 - (outerRadius / dist);
				//std::cout << oddsToStop << std::endl;
				if (((double) std::rand() / (RAND_MAX)) < oddsToStop)
					continue;
			}

            Vector2 adjustedDest = Vector2(destinationVector.x - (int)destinationVector.x % 5,
                                           destinationVector.y - (int)destinationVector.y % 5);


            if (existing.count(adjustedDest))
                continue;

			float distFromCenterRatio = std::pow(innerRadius / (destinationVector - center).magnitude(), 2);
			RGBA color = gradientFunction(distFromCenterRatio);

            fw.drawLine(current.point, destinationVector, color);
            points.push_back(HorizonPoint { destinationVector, newAngle } );
            existing.insert(adjustedDest);
        }
    }
}

int main(int argc, char* argv[]) {
    //int fps = 60;
    //double msPerFrame = (1000.0/fps);
    //double delta = 1.0/fps;

    int height = 4000;
    int width = 8000;

    Framework fw(height, width);

    
    //SDL_Event event;
    //const Uint8* state = SDL_GetKeyboardState(nullptr);
    
    fw.clear();                     // clear the last frame

    draw_black_hole(fw, width * height, width, height);

    fw.render();
    fw.saveImage();

    std::cout << "done" << std::endl;

    return 0;
}