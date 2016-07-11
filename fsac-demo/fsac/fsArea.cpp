#include "fsArea.h"

#include <algorithm>

fsArea::fsArea() : index(0), showObservationWindow(false) {

}

fsArea::fsArea(const int &cindex) : index(cindex), showObservationWindow(false) {

}

fsBlock* fsArea::addBlock(const int &cindex) {
	fsBlock *newBlock = new fsBlock(cindex, this);
	blocks.push_back(newBlock);

	return newBlock;
}

bool fsArea::deleteBlock(const int &cindex) {
	bool deleted = false;

	auto iter = std::find_if(blocks.begin(), blocks.end(), [cindex](fsBlock *cblock) {
		return (cblock->index == cindex);
	});

	if (iter != blocks.end()) {
		if (*iter) {
			delete *iter;
		}

		deleted = true;
		blocks.erase(iter);
	}

	return deleted;
}

void fsArea::setSize(const int &cw, const int &ch) {
	width = cw;
	height = ch;
}

void fsArea::simulate() {
	for (auto iter = blocks.begin(); iter != blocks.end(); ++iter) {
		(*iter)->simulate();
	}
}