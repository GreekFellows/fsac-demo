#include <algorithm>

#include "fsPopulation.h"
#include "fsEffect.h"
#include "fsChaosCalls.h"

fsPopulation::fsPopulation() : index(-1), name(""), size(0), color(rand() % 0xffffff), showObservationWindow(false) {

}

fsPopulation::fsPopulation(const int &cnumber, const std::string &cname, fsBlock *cblock) : index(cnumber), name(cname), size(0), block(cblock), color(rand() % 0xffffff), showObservationWindow(false) {

}

void fsPopulation::simulate() {
	// receive block-bound effects
	auto typeAtoxins = block->getEffectsById(fsEffectId::TOXIN_A);
	auto typeBtoxins = block->getEffectsById(fsEffectId::TOXIN_B);
	auto typeCtoxins = block->getEffectsById(fsEffectId::TOXIN_C);

	if (hasAlleleWithId(fsAlleleID::ST_A)) {
		if (!hasAlleleWithId(fsAlleleID::TRES_A)) {
			for (auto iter = typeAtoxins.begin(); iter != typeAtoxins.end(); ++iter) {
				size *= (*iter).doubles[0];
			}
		}
	}
	else if (hasAlleleWithId(fsAlleleID::ST_B)) {
		if (!hasAlleleWithId(fsAlleleID::TRES_B)) {
			for (auto iter = typeBtoxins.begin(); iter != typeBtoxins.end(); ++iter) {
				size *= (*iter).doubles[0];
			}
		}
	}
	else if (hasAlleleWithId(fsAlleleID::ST_C)) {
		if (!hasAlleleWithId(fsAlleleID::TRES_B)) {
			for (auto iter = typeCtoxins.begin(); iter != typeCtoxins.end(); ++iter) {
				size *= (*iter).doubles[0];
			}
		}
	}

	// recalculate population data
	if (hasAlleleWithId(fsAlleleID::R_PPLUS)) {
		size *= 1.12;
	}
	else if (hasAlleleWithId(fsAlleleID::R_PLUS)) {
		size *= 1.05;
	}
	else if (hasAlleleWithId(fsAlleleID::R_NORMAL)) {
		size *= 1.01;
	}
	size += 1;

	// impose block-bound effects
	if (hasAlleleWithId(fsAlleleID::TREL_A)) {
		block->addEffect(fsEffect(fsEffectId::TOXIN_A, {}, {
			fsChaosCalls::spitDouble(0.8, 1)
		}));
	}
	if (hasAlleleWithId(fsAlleleID::TREL_B)) {
		block->addEffect(fsEffect(fsEffectId::TOXIN_B, {}, {
			fsChaosCalls::spitDouble(0.8, 1)
		}));
	}
	if (hasAlleleWithId(fsAlleleID::TREL_C)) {
		block->addEffect(fsEffect(fsEffectId::TOXIN_C, {}, {
			fsChaosCalls::spitDouble(0.8, 1)
		}));
	}
}

fsGene* fsPopulation::addGeneById(const fsGeneID &cid) {
	fsGene *newGene = nullptr;
	
	if (!hasGeneWithId(cid)) {
		newGene = new fsGene(cid);
		genome.push_back(newGene);
		geneLookupTable[cid] = true;
	}

	return newGene;
}

bool fsPopulation::deleteGeneById(const fsGeneID &cid) {
	bool deleted = false;

	auto iter = std::find_if(genome.begin(), genome.end(), [cid](fsGene *cgene) {
		return (cgene->id == cid);
	});

	if (iter != genome.end()) {
		if (*iter) {
			delete *iter;
		}

		deleted = true;
		genome.erase(iter);
		geneLookupTable.erase(cid);
	}

	return deleted;
}

fsGene* fsPopulation::getGeneById(const fsGeneID &cid) {
	auto iter = std::find_if(genome.begin(), genome.end(), [cid](fsGene *cgene) {
		return (cgene->id == cid);
	});

	return (iter == genome.end() ? nullptr : *iter);
}

fsAllele* fsPopulation::addAlleleById(const fsAlleleID &cid) {
	fsAllele *newAllele = nullptr;

	fsGeneID gid = fsAllele::findAlleleInfo(cid).geneID;
	if (hasGeneWithId(gid)) {
		newAllele = getGeneById(gid)->addAlleleById(cid);
	}
	else {
		newAllele = addGeneById(gid)->addAlleleById(cid);
	}

	alleleLookupTable[cid] = true;
	return newAllele;
}

// http://stackoverflow.com/a/12261945/1517227
// http://stackoverflow.com/a/7624668/1517227
bool fsPopulation::deleteAllelesById(const fsAlleleID &cid) {
	bool deleted = false;

	fsGeneID gid = fsAllele::findAlleleInfo(cid).geneID;
	if (hasGeneWithId(gid)) {
		if (getGeneById(gid)->deleteAllelesById(cid)) {
			deleted = true;
			alleleLookupTable.erase(cid);
		}
	}

	return deleted;
}

std::vector<fsAllele*> fsPopulation::getAllelesById(const fsAlleleID &cid) {
	std::vector<fsAllele*> result;

	fsGeneID gid = fsAllele::findAlleleInfo(cid).geneID;
	if (hasGeneWithId(gid)) {
		result = getGeneById(gid)->getAllelesById(cid);
	}

	return result;
}

void fsPopulation::updateGeneLookupTable() {
	geneLookupTable.clear();

	for (auto iter = genome.begin(); iter != genome.end(); ++iter) {
		geneLookupTable[(*iter)->id] = true;
	}
}

void fsPopulation::updateAlleleLookupTable() {
	alleleLookupTable.clear();

	for (auto giter = genome.begin(); giter != genome.end(); ++giter) {
		for (auto aiter = (*giter)->alleles.begin(); aiter != (*giter)->alleles.end(); ++aiter) {
			alleleLookupTable[(*aiter)->id] = true;
		}
	}
}

bool fsPopulation::hasGeneWithId(const fsGeneID &cid) {
	bool result;

	try {
		result = geneLookupTable.at(cid);
	}
	catch (std::out_of_range) {
		result = false;
	}

	return result;
}

bool fsPopulation::hasAlleleWithId(const fsAlleleID &cid) {
	bool result;

	try {
		result = alleleLookupTable.at(cid);
	}
	catch (std::out_of_range) {
		result = false;
	}

	return result;
}

std::string fsPopulation::makeWindowTitle() const {
	return ("population " + std::to_string(index));
}