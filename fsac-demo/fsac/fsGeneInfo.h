#pragma once

#include <string>
#include <vector>

#include "fsAlleleInfo.h"
#include "fsGeneID.h"
#include "fsAlleleID.h"

class fsGeneInfo {
public:

	fsGeneID id;
	std::string name, desc;
	std::vector<fsAlleleID> alleleIDs;

	bool multipleAlleleCausesDeath = { false };	// need a better naming...

	fsGeneInfo();
	fsGeneInfo(const fsGeneID&, const std::string&, const std::string&, const std::initializer_list<fsAlleleID>&);

	std::string makeWindowTitle() const;

};

const fsGeneInfo testGI = fsGeneInfo(
	fsGeneID::TESTGENE,
	"test name 2",
	"test desc",
	{ fsAlleleID::TESTALLELE }
);
const fsGeneInfo speciesTypeGI = fsGeneInfo(
	fsGeneID::SPECIES_TYPE,
	"species type",
	"the type of this species",
	{ fsAlleleID::ST_A, fsAlleleID::ST_B, fsAlleleID::ST_C }
	);
const fsGeneInfo resourceDependencyGI = fsGeneInfo(
	fsGeneID::RESOURCE_DEPENDENCY,
	"resource dependency",
	"the type of resource this species depends on",
	{ fsAlleleID::RD_ALPHA, fsAlleleID::RD_BETA }
	);
const fsGeneInfo reproductionGI = fsGeneInfo(
	fsGeneID::REPRODUCTION,
	"reproduction",
	"the rate at which this species reproduces",
	{ fsAlleleID::R_NORMAL, fsAlleleID::R_PLUS, fsAlleleID::R_PPLUS }
	);
const fsGeneInfo toxinReleaseGI = fsGeneInfo(
	fsGeneID::TOXIN_RELEASE,
	"toxin release",
	"the type of the species-specific toxin released by this species",
	{ fsAlleleID::TREL_A, fsAlleleID::TREL_B, fsAlleleID::TREL_C }
	);
const fsGeneInfo toxinResistanceGI = fsGeneInfo(
	fsGeneID::TOXIN_RESISTANCE,
	"toxin resistance",
	"the type of the species-specific toxin against which this species has resistance",
	{ fsAlleleID::TRES_A, fsAlleleID::TRES_B, fsAlleleID::TRES_C }
	);