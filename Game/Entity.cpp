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
#include "Entity.h"
#include "GameGlobal.h"
#include "GameBoard.h"

#include <iostream>

namespace game {

Entity::Entity() :
	m_updatable(false),

	m_position(),
	m_rotation(),
	m_scale(1.0f),

	m_id(0),
	m_location(0) {}

void Entity::OnJoined(size_t id, size_t location) {
	m_id = id;
	m_location = location;

	// for test
	std::cout << "Join: " << this << ", ID: " << id << ", Location: " << location << std::endl;
}

void Entity::OnUpdate(float dt) {}

void Entity::OnKicking() {
	m_id = 0;
}

void Entity::OnPositionChanged() {}

void Entity::OnRotationChanged() {}

void Entity::OnScaleChanged() {}

void Entity::KickSelf() const {
	Global::data.board->KickEntity(m_location);
}

}
