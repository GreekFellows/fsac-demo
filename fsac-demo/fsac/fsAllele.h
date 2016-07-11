#pragma once

#include "fsGene.h"
#include "fsAlleleInfo.h"

class fsGene;

class fsAllele {
public:

	fsAlleleID id;
	fsGene *gene;

	fsAllele();
	fsAllele(const fsAlleleID&, fsGene*);

	static fsAlleleInfo findAlleleInfo(const fsAlleleID&);

};