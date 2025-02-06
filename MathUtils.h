#pragma once

#include <cmath>

class MathUtils {
public:
	static float randomFloat() {
		return rand() / (RAND_MAX + 1.0f);
	}
};