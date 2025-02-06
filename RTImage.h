#pragma once

#include <vector>
#include <cmath>
#include "color.h"

#include "interval.h"

class RTImage {
private:
	int imageWidth = 0;
	int imageHeight = 0;

	std::vector <std::vector<color>> pixels;

public:
	RTImage(int imageWidth, int imageHeight) {
		this->imageWidth = imageWidth;
		this->imageHeight = imageHeight;
	}

	void setPixel(int x, int y, float r, float g, float b, int samplesPerPixel) {
		float scale = 1.0f / samplesPerPixel;

		r = sqrt(scale * r);
		g = sqrt(scale * g);
		b = sqrt(scale * b);

		static const interval intensity(0.000, 0.999);
		int rbyte = int(256 * intensity.clamp(r));
		int gbyte = int(256 * intensity.clamp(g));
		int bbyte = int(256 * intensity.clamp(b));
	}
};