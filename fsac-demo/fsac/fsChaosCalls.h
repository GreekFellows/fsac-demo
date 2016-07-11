#pragma once

#include <random>

class fsChaosCalls {
	
	static std::minstd_rand engine;

public:

	static int spitInt(const int &min, const int &max) {
		return ((double)engine() / (double)(max - min) + min);
	}

	static double spitDouble(const double &min, const double &max) {
		return ((double)engine() / (max - min) + min);
	}

};