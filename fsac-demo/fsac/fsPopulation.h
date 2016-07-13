#pragma once

#include <string>
#include <vector>
#include <map>

#include "fsGene.h"
#include "fsBlock.h"

class fsBlock;

class fsPopulation {
public:

	int index;
	std::string name;
	int color;

	long long size;
	std::vector<fsGene*> genome;
	fsBlock *block;
	std::vector<fsPopulation*> linkedPopulations;	// e.g. when population spreads to others. speciating mutations are less likely to occur in linked populations. once they do, the link is broken.

	std::map<fsGeneID, bool> geneLookupTable;
	std::map<fsAlleleID, bool> alleleLookupTable;

	bool showObservationWindow = false;

	fsPopulation();
	fsPopulation(const int&, const std::string&, fsBlock*);

	fsGene* addGeneById(const fsGeneID&);
	bool deleteGeneById(const fsGeneID&);
	fsGene* getGeneById(const fsGeneID&);

	fsAllele* addAlleleById(const fsAlleleID&);
	bool deleteAllelesById(const fsAlleleID&);
	std::vector<fsAllele*> getAllelesById(const fsAlleleID&);

	void updateGeneLookupTable();
	void updateAlleleLookupTable();
	// void updateAlleleLookupTableByGeneId(const fsGeneID&);	// selective update

	bool hasGeneWithId(const fsGeneID&);
	bool hasAlleleWithId(const fsAlleleID&);

	void simulate();

	std::string makeWindowTitle() const;

};