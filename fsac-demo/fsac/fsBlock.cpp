#include <algorithm>

#include "fsBlock.h"

fsBlock::fsBlock() : index(0), showObservationWindow(false) {

}

fsBlock::fsBlock(const int &cindex, fsArea *carea) : index(cindex), area(carea), showObservationWindow(false) {

}

void fsBlock::addEffect(const fsEffect &ceffect) {
	effects.push_back(ceffect);
}

bool fsBlock::deleteEffect(fsEffect ceffect) {
	bool deleted = false;

	for (auto iter = effects.begin(); iter != effects.end(); ++iter) {
		if (*iter == ceffect) {
			deleted = true;
			effects.erase(iter);
			break;
		}
	}

	return deleted;
}

std::vector<fsEffect> fsBlock::getEffectsById(const fsEffectId &cid) {
	std::vector<fsEffect> effects;

	for (auto iter = effects.begin(); iter != effects.end(); ++iter) {
		if ((*iter).id == cid) {
			effects.push_back(*iter);
		}
	}

	return effects;
}

fsPopulation* fsBlock::addPopulation(const int &cindex, const std::string &cname) {
	fsPopulation *newPopulation = new fsPopulation(cindex, cname, this);
	populations.push_back(newPopulation);

	return newPopulation;
}

bool fsBlock::deletePopulation(const int &cindex) {
	bool deleted = false;

	auto iter = std::find_if(populations.begin(), populations.end(), [cindex](fsPopulation *cpopulation) {
		return (cpopulation->index == cindex);
	});

	if (iter != populations.end()) {
		if (*iter) {
			delete *iter;
		}

		deleted = true;
		populations.erase(iter);
	}

	return deleted;
}

void fsBlock::simulate() {
	for (auto iter = populations.begin(); iter != populations.end(); ++iter) {
		(*iter)->simulate();
	}

	// by default delete all effects with affectCount > 0 (effect has stayed around for at least one simulation cycle), otherwise increment affectCount
	effects.erase(std::remove_if(effects.begin(), effects.end(), [](fsEffect ceffect) {
		return (ceffect.affectCount++);
	}), effects.end());
}

std::string fsBlock::makeWindowTitle() const {
	return ("block " + std::to_string(index));
}