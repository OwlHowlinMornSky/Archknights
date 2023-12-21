#include "UnitFactory.h"
#include "Unit.h"
#include "GameGlobal.h"
#include "GameBoard.h"

#include <iostream>

namespace game {

UnitFactory::UnitFactory() {
	m_cnt = 0.0f;
}

UnitFactory::~UnitFactory() {}

std::shared_ptr<Unit> UnitFactory::JoinOneUnit() {
	std::shared_ptr<Unit> unit;
	if (m_storedUnits.empty()) {
		std::cout << "Factory Made One" << std::endl; // for test
		unit = CreateUnit();
		m_units.push_back(unit);
		unit->UnitOnCreated(*this, m_units.size()); // ProductID 从 1 开始，且是位置加一
	}
	else {
		std::cout << "Factory Reset One" << std::endl; // for test
		unit = m_units[m_storedUnits.top() - 1]; // ProductID 从 1 开始，所以位置要减一
		m_storedUnits.pop();
		unit->UnitReset();
	}
	Global::instance()->data.board->JoinEntity(unit);
	return unit;
}

void UnitFactory::ReturnUnit(Unit* unit) {
	std::cout << "Factory Recycled One" << std::endl; // for test
	m_storedUnits.push(unit->m_productID);
}

void UnitFactory::OnJoined(size_t id, size_t location) {
	Parent::OnJoined(id, location);
	JoinOneUnit(); // for test
	JoinOneUnit(); // for test
	JoinOneUnit(); // for test
	m_updatable = true; // for test
}

void UnitFactory::OnUpdate(float dt) {
	if (m_storedUnits.size() == m_units.size()) {
		JoinOneUnit();
	}
	//m_cnt += dt;
	//while (m_cnt >= 1.0f) {
	//	m_cnt -= 1.0f;
	//	JoinOneUnit();
	//}
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
