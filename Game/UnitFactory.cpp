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
#include "IGameBoard.h"

#include <iostream>

namespace game {

UnitFactory::UnitFactory() {
}

UnitFactory::~UnitFactory() {
	Clear();
}

std::shared_ptr<Unit> UnitFactory::JoinOneUnit() {
	std::shared_ptr<Unit> unit;
	if (m_storedUnits.empty()) {
		unit = CreateUnit();
		m_units.push_back(unit);
		unit->UnitOnCreated(*this, m_units.size()); // ProductID 从 1 开始，且是位置加一
	}
	else {
		unit = m_units[m_storedUnits.top() - 1]; // ProductID 从 1 开始，所以位置要减一
		m_storedUnits.pop();
		unit->UnitReset();
	}
	Global::data.board->JoinEntity(unit);
	return unit;
}

void UnitFactory::ReturnUnit(Unit* unit) {
	m_storedUnits.push(unit->m_productID);
}

void UnitFactory::Clear() {
	m_units.clear();
	while (!m_storedUnits.empty())
		m_storedUnits.pop();
	return;
}

} // namespace game
