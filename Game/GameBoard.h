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

#include "GameGlobal.h"
#include "Entity.h"
#include <deque>
#include <stack>
#include "RootLoader.h"
#include "UnitFactory.h"
#include "MessageRepeater.h"
#include <box2d/box2d.h>

namespace game {

class GameBoard final {
public:
	GameBoard();
	~GameBoard();

	void setup();
	void clear();

public:
	bool isEmpty();
	void setPause(bool pause);

	void Update(float dt);

	/**
	 * @brief 添加一个工厂。
	 * @param factory 工厂。
	 * @return 工厂id（从1开始）。
	*/
	size_t AddFactory(std::shared_ptr<UnitFactory> factory);
	void JoinEntityFromFactory(size_t id);

	void JoinEntity(std::shared_ptr<Entity> entity);
	void KickEntity(size_t location);

	//b2World& World() {
	//	return *m_physics;
	//}

	std::shared_ptr<Entity> EntityAt(size_t location);

protected:
	size_t m_idCnt;

	std::deque<std::shared_ptr<Entity>> m_entities;
	std::stack<size_t> m_emptyLocation;
	std::shared_ptr<RootLoader> m_rootLoader;

	std::deque<std::shared_ptr<UnitFactory>> m_factories;
	std::unique_ptr<b2World> m_physics;
	std::unique_ptr<MessageRepeater> m_repeater;
	
	bool m_paused;
};

} // namespace game
