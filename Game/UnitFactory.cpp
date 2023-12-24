/*
*    Archknights
*
*    Copyright (C) 2023-2024  Tyler Parret True
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published
*    by the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* @Authors
*    Tyler Parret True <mysteryworldgod@outlook.com><https://github.com/OwlHowlinMornSky>
*/
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
	Global::data.board->JoinEntity(unit);
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
	auto& board = Global::data.board;
	for (std::shared_ptr<Unit>& u : m_units)
		if (u->m_id)
			board->KickEntity(u->m_location);
	m_units.clear();
	while (!m_storedUnits.empty())
		m_storedUnits.pop();
	return Parent::OnKicking();
}

} // namespace game
