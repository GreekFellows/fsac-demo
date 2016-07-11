#pragma once

#include <vector>

#include "fsAllele.h"
#include "fsGeneInfo.h"

class fsAllele;

class fsGene {
public:

	fsGeneID id;
	std::vector<fsAllele*> alleles;

	fsGene();
	fsGene(const fsGeneID&);

	static fsGeneInfo findGeneInfo(const fsGeneID&);

	fsAllele* addAlleleById(const fsAlleleID&);
	bool deleteAllelesById(const fsAlleleID&);
	std::vector<fsAllele*> getAllelesById(const fsAlleleID&);	// there might be multiple alleles of the same kind

};