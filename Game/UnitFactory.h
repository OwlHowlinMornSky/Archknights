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
#pragma once

#include "Entity.h"
#include <stack>
#include <memory>

namespace game {

class Unit;

class UnitFactory :
	public Entity {
	using Parent = game::Entity;
public:
	UnitFactory();
	virtual ~UnitFactory() override;

public:
	std::shared_ptr<Unit> JoinOneUnit();
	void ReturnUnit(Unit* unit);

protected:
	virtual std::shared_ptr<Unit> CreateUnit() const = 0;

protected:
	std::deque<std::shared_ptr<Unit>> m_units;
	std::stack<size_t> m_storedUnits;
	float m_cnt;

// 继承的
public:
	virtual void OnJoined(size_t id, size_t location) override;
	virtual void OnUpdate(float dt) override;
	virtual void OnKicking() override;
};

}
