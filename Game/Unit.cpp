#include "Unit.h"

namespace game {

Unit::Unit() :
	m_productID(0) {
	m_updatable = true;
}

Unit::~Unit() {}

void Unit::UnitOnCreated(UnitFactory& factory, size_t productID) {
	m_factory(factory);
	m_productID = productID;
}

void Unit::OnKicking() {
	m_factory->ReturnUnit(this);
	return Parent::OnKicking();
}

}
