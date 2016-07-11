#pragma once

#include <string>

#include "fsGeneInfo.h"
#include "fsAlleleID.h"
#include "fsGeneID.h"

class fsAlleleInfo {
public:

	fsAlleleID id;
	std::string name, desc;
	fsGeneID geneID;

	fsAlleleInfo();
	fsAlleleInfo(const fsAlleleID&, const std::string&, const std::string&, const fsGeneID&);

};

const fsAlleleInfo testAI = fsAlleleInfo(
	fsAlleleID::TESTALLELE,
	"test name 1",
	"test desc",
	fsGeneID::TESTGENE
	);

const fsAlleleInfo stA_AI = fsAlleleInfo(
	fsAlleleID::ST_A,
	"STA",
	"type A species",
	fsGeneID::SPECIES_TYPE
	);
const fsAlleleInfo stB_AI = fsAlleleInfo(
	fsAlleleID::ST_B,
	"STB",
	"type B species",
	fsGeneID::SPECIES_TYPE
	);
const fsAlleleInfo stC_AI = fsAlleleInfo(
	fsAlleleID::ST_C,
	"STC",
	"type C species",
	fsGeneID::SPECIES_TYPE
	);

const fsAlleleInfo rdAlpha_AI = fsAlleleInfo(
	fsAlleleID::RD_ALPHA,
	"RDƒ¿",
	"dependent on resource ƒ¿",
	fsGeneID::RESOURCE_DEPENDENCY
	);
const fsAlleleInfo rdBeta_AI = fsAlleleInfo(
	fsAlleleID::RD_BETA,
	"RDƒÀ",
	"dependent on resource ƒÀ",
	fsGeneID::RESOURCE_DEPENDENCY
	);

const fsAlleleInfo rN_AI = fsAlleleInfo(
	fsAlleleID::R_NORMAL,
	"RN",
	"reproduction rate at 1.01",
	fsGeneID::REPRODUCTION
	);
const fsAlleleInfo rP_AI = fsAlleleInfo(
	fsAlleleID::R_PLUS,
	"R+",
	"reproduction rate at 1.05",
	fsGeneID::REPRODUCTION
	);
const fsAlleleInfo rPP_AI = fsAlleleInfo(
	fsAlleleID::R_PPLUS,
	"R++",
	"reproduction rate at 1.12",
	fsGeneID::REPRODUCTION
	);

const fsAlleleInfo trelA_AI = fsAlleleInfo(
	fsAlleleID::TREL_A,
	"TRelA",
	"releases type A species-specific toxin",
	fsGeneID::TOXIN_RELEASE
	);
const fsAlleleInfo trelB_AI = fsAlleleInfo(
	fsAlleleID::TREL_B,
	"TRelB",
	"releases type B species-specific toxin",
	fsGeneID::TOXIN_RELEASE
	);
const fsAlleleInfo trelC_AI = fsAlleleInfo(
	fsAlleleID::TREL_C,
	"TRelC",
	"releases type C species-specific toxin",
	fsGeneID::TOXIN_RELEASE
	);

const fsAlleleInfo tresA_AI = fsAlleleInfo(
	fsAlleleID::TRES_A,
	"TResA",
	"resistant to type A species-specific toxin",
	fsGeneID::TOXIN_RESISTANCE
	);
const fsAlleleInfo tresB_AI = fsAlleleInfo(
	fsAlleleID::TRES_B,
	"TResB",
	"resistant to type B species-specific toxin",
	fsGeneID::TOXIN_RESISTANCE
	);
const fsAlleleInfo tresC_AI = fsAlleleInfo(
	fsAlleleID::TRES_C,
	"TResC",
	"resistant to type C species-specific toxin",
	fsGeneID::TOXIN_RESISTANCE
	);