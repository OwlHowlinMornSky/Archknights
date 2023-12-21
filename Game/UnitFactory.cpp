#include "UnitFactory.h"
#include "Unit.h"
#include "GameGlobal.h"
#include "GameBoard.h"

namespace game {

UnitFactory::UnitFactory() {}

UnitFactory::~UnitFactory() {}

std::shared_ptr<Unit> UnitFactory::JoinOneUnit() {
	std::shared_ptr<Unit> unit;
	if (m_storedUnits.empty()) {
		unit = CreateUnit();
		m_units.push_back(unit);
		unit->UnitOnCreated(*this, m_units.size());
	}
	else {
		unit = m_units[m_storedUnits.top()];
		m_storedUnits.pop();
		unit->UnitReset();
	}
	Global::instance()->data.board->JoinEntity(unit);
	return unit;
}

void UnitFactory::ReturnUnit(size_t productID) {
	m_storedUnits.push(productID);
}

void UnitFactory::OnKicking() {
	auto& board = Global::instance()->data.board;
	for (std::shared_ptr<Unit>& u : m_units)
		if (u->m_id)
			board->KickEntity(u->m_location);
	m_units.clear();
	while (!m_storedUnits.empty())
		m_storedUnits.pop();
	return Parent::OnKicking();
}

}
