/*
*    Mystery Engine
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
#include "IGameBoard.h"
#include "MsgResult.h"

#include <iostream>

namespace ME {

Entity::Entity() :
	m_updatable(false),

	m_position(),
	m_rotation(),
	m_scale(1.0f),

	m_id(0),
	m_location(0) {}

void Entity::BasicOnJoined(EntityIdType id, EntityLocationType location) {
	m_id = id;
	m_location = location;

	// for test
	std::cout << "Join: " << this << ", ID: " << id << ", Location: " << location << std::endl;
	return OnJoined();
}

void Entity::BasicOnKicking() {
	OnKicking();
	std::cout << "Kick: " << this << ", ID: " << m_id << ", Location: " << m_location << std::endl;
	m_id = 0;
}

void Entity::OnJoined() {}

void Entity::OnKicking() {}

MsgResultType Entity::ReceiveMessage(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	return MsgResult::Unsubscribe;
}

void Entity::OnPositionChanged() {}

void Entity::OnRotationChanged() {}

void Entity::OnScaleChanged() {}

void Entity::KickSelf() const {
	GameGlobal::board->KickEntity(m_location);
}

} // namespace game
