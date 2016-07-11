#pragma once

#include <vector>

enum fsEffectId {
	TESTEFFECT = -1,

	// species-specific toxin
	TOXIN_A, TOXIN_B, TOXIN_C
};

class fsEffect {
public:

	fsEffectId id;
	std::vector<int> ints;
	std::vector<double> doubles;

	int affectCount;

	fsEffect();
	fsEffect(const fsEffectId&, const std::initializer_list<int>&, const std::initializer_list<double>&);

	bool operator==(const fsEffect&);

};