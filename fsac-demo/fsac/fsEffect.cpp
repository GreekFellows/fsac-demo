#include "fsEffect.h"

fsEffect::fsEffect() : id(fsEffectId::TESTEFFECT), affectCount(0) {

}

fsEffect::fsEffect(const fsEffectId &cid, const std::initializer_list<int> &cints, const std::initializer_list<double> &cdoubles) : id(cid), ints(cints), doubles(cdoubles), affectCount(0) {

}

bool fsEffect::operator==(const fsEffect &rhs) {
	return (id == rhs.id && ints == rhs.ints && doubles == rhs.doubles && affectCount == rhs.affectCount);
}