#include "../include/rgba.h"


RGBA::RGBA(int red, int green, int blue) : red(red), green(green), blue(blue), alpha(255) {}

RGBA::RGBA(int red, int green, int blue, int alpha) : red(red), green(green), blue(blue), alpha(alpha) {}
	
void RGBA::increaseBrightness() {
	this->red++;
	this->blue++;
	this->green++;
}

	
std::function<const RGBA(const float)> RGBA::generateGradientFunction(RGBA start, RGBA end) {
	return [start, end](float ratio) {
		if (ratio > 1 || ratio < 0)
			return RGBA(0, 0, 0, 0);
		
		return RGBA(
			start.red + (end.red - start.red) * ratio,
			start.green + (end.green - start.green) * ratio,
			start.blue + (end.blue - start.blue) * ratio,
			start.alpha + (end.alpha - start.alpha) * ratio
		);
	};
}