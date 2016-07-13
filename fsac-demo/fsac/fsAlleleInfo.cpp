#include "fsAlleleInfo.h"

fsAlleleInfo::fsAlleleInfo() : id(TESTALLELE), name("test name"), desc("test desc") {

}

fsAlleleInfo::fsAlleleInfo(const fsAlleleID &cid, const std::string &cname, const std::string &cdesc, const fsGeneID &cgene) : id(cid), name(cname), desc(cdesc), geneID(cgene) {

}

std::string fsAlleleInfo::makeWindowTitle() const {
	return (name + " allele");
}