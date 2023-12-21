#include "Unit.h"

namespace game {

Unit::Unit() {}

Unit::~Unit() {}

void Unit::UnitOnCreated(UnitFactory& factory, size_t productID) {
	m_factory(factory);
	m_productID = productID;
}

void Unit::OnKicking() {
	m_factory->ReturnUnit(m_productID);
	return Parent::OnKicking();
}

}
