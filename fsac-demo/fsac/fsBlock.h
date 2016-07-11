#pragma once

#include <vector>

#include "fsArea.h"
#include "fsPopulation.h"
#include "fsEffect.h"

class fsArea;
class fsPopulation;

class fsBlock {
public:

	int index;

	std::vector<fsPopulation*> populations;
	std::vector<fsEffect> effects;
	fsArea* area;

	bool showObservationWindow = false;

	fsBlock();
	fsBlock(const int&, fsArea*);
	
	void addEffect(const fsEffect&);
	bool deleteEffect(fsEffect);	// this changes elements in the effects vector, so MUST NOT be called before all populations in this block are simulate()d
	std::vector<fsEffect> getEffectsById(const fsEffectId&);

	fsPopulation* addPopulation(const int&, const std::string&);
	bool deletePopulation(const int&);

	void simulate();

};