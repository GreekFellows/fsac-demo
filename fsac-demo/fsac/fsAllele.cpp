#include "fsAllele.h"

fsAllele::fsAllele() : gene(nullptr) {

}

fsAllele::fsAllele(const fsAlleleID &cid, fsGene *cgene) : id(cid), gene(cgene) {
	
}

// named return-value optimization?
fsAlleleInfo fsAllele::findAlleleInfo(const fsAlleleID &findId) {
	fsAlleleInfo result;

	switch (findId) {
	case fsAlleleID::ST_A:
		result = stA_AI;
		break;
	case fsAlleleID::ST_B:
		result = stB_AI;
		break;
	case fsAlleleID::ST_C:
		result = stC_AI;
		break;
	case fsAlleleID::RD_ALPHA:
		result = rdAlpha_AI;
		break;
	case fsAlleleID::RD_BETA:
		result = rdBeta_AI;
		break;
	case fsAlleleID::R_NORMAL:
		result = rN_AI;
		break;
	case fsAlleleID::R_PLUS:
		result = rP_AI;
		break;
	case fsAlleleID::R_PPLUS:
		result = rPP_AI;
		break;
	case fsAlleleID::TREL_A:
		result = trelA_AI;
		break;
	case fsAlleleID::TREL_B:
		result = trelB_AI;
		break;
	case fsAlleleID::TREL_C:
		result = trelC_AI;
		break;
	case fsAlleleID::TRES_A:
		result = tresA_AI;
		break;
	case fsAlleleID::TRES_B:
		result = tresB_AI;
		break;
	case fsAlleleID::TRES_C:
		result = tresC_AI;
		break;
	}

	return result;
}