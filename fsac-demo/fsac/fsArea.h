#pragma once

#include <string>
#include <vector>

#include "fsBlock.h"

class fsBlock;

class fsArea {
public:

	int index;

	int width, height;

	std::string name;
	std::vector<fsBlock*> blocks;

	bool showObservationWindow = false;

	fsArea();
	fsArea(const int&);

	fsBlock* addBlock(const int&);
	bool deleteBlock(const int&);

	void setSize(const int&, const int&);

	void simulate();

	std::string makeWindowTitle() const;

};