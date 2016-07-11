#include "fsGeneInfo.h"

fsGeneInfo::fsGeneInfo() : id(TESTGENE), name("test name"), desc("test desc") {

}

fsGeneInfo::fsGeneInfo(const fsGeneID &cid, const std::string &cname, const std::string &cdesc, const std::initializer_list<fsAlleleID> &calleles) : id(cid), name(cname), desc(cdesc), alleleIDs(calleles) {
	
}