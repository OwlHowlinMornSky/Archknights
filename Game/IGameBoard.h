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
#include "UnitFactory.h"
#include <memory>

namespace game {

class IGameBoard {
public:
	IGameBoard() = default;
	virtual ~IGameBoard() = default;

	virtual void setup() = 0;
	virtual void clear() = 0;

public:
	virtual bool isEmpty() = 0;
	virtual void setPause(bool pause) = 0;

	virtual void Update(float dt) = 0;

	/**
	 * @brief 添加一个工厂。
	 * @param factory 工厂。
	 * @return 工厂id（从1开始）。
	*/
	virtual size_t AddFactory(std::shared_ptr<UnitFactory> factory) = 0;
	virtual void JoinEntityFromFactory(size_t id) = 0;

	virtual void JoinEntity(std::shared_ptr<Entity> entity) = 0;
	virtual void KickEntity(size_t location) = 0;

	virtual std::shared_ptr<Entity> EntityAt(size_t location) = 0;
};

}
