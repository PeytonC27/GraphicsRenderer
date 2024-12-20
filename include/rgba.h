#pragma once

#include <functional>

class RGBA {
private:
	
public:
	
	int red;
	int green;
	int blue;
	int alpha;

	RGBA(int red, int green, int blue);
	RGBA(int red, int green, int blue, int alpha);
	
	void increaseBrightness();
	static std::function<const RGBA(const float)> generateGradientFunction(RGBA start, RGBA end);
};