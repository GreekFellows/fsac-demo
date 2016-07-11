#include <algorithm>

#include "fsGene.h"

fsGene::fsGene() {

}

fsGene::fsGene(const fsGeneID &cid) : id(cid) {

}

// named return-value optimization?
fsGeneInfo fsGene::findGeneInfo(const fsGeneID &findId) {
	fsGeneInfo result;

	switch (findId) {
	case fsGeneID::TESTGENE:
		result = testGI;
		break;

	case fsGeneID::SPECIES_TYPE:
		result = speciesTypeGI;
		result.multipleAlleleCausesDeath = true;
		break;

	case fsGeneID::RESOURCE_DEPENDENCY:
		result = resourceDependencyGI;
		break;

	case fsGeneID::REPRODUCTION:
		result = reproductionGI;
		break;

	case fsGeneID::TOXIN_RELEASE:
		result = toxinReleaseGI;
		break;

	case fsGeneID::TOXIN_RESISTANCE:
		result = toxinResistanceGI;
		break;
	}

	return result;
}

fsAllele* fsGene::addAlleleById(const fsAlleleID &cid) {
	fsAllele *newAllele = new fsAllele(cid, this);
	alleles.push_back(newAllele);

	return newAllele;
}

bool fsGene::deleteAllelesById(const fsAlleleID &cid) {
	bool deleted = false;

	alleles.erase(std::remove_if(alleles.begin(), alleles.end(), [cid, &deleted](fsAllele *callele) {
		bool foundAllele = callele->id == cid;
		if (foundAllele) {
			deleted = true;
			if (callele != nullptr) {
				delete callele;
			}
		}
		return foundAllele;
	}), alleles.end());

	return deleted;
}

std::vector<fsAllele*> fsGene::getAllelesById(const fsAlleleID &cid) {
	std::vector<fsAllele*> result;

	for (auto iter = alleles.begin(); iter != alleles.end(); ++iter) {
		if ((*iter)->id == cid) {
			result.push_back(*iter);
		}
	}

	return result;
}