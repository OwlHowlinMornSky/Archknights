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
#include "MsgResult.h"

#include <iostream>

namespace Game {

Entity::Entity() :
	m_position{ 0.0f, 0.0f },
	m_rotation(0.0f),
	m_scale{ 1.0f, 1.0f },

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

void Entity::FixedUpdate(float dt) {}

EntityIdType Entity::getID() const {
	return m_id;
}

EntityLocationType Entity::getLocation() const {
	return m_location;
}

const float* Entity::getPosition() const {
	return m_position;
}

void Entity::setPosition(float x, float y) {
	m_position[0] = x;
	m_position[1] = y;
	OnPositionChanged();
}

const float* Entity::getScale() const {
	return m_scale;
}

void Entity::setScale(float x, float y) {
	m_scale[0] = x;
	m_scale[1] = y;
	OnScaleChanged();
}

float Entity::getRotation() const {
	return m_rotation;
}

void Entity::setRotation(float rot) {
	m_rotation = rot;
	OnRotationChanged();
}

void Entity::OnPositionChanged() {}

void Entity::OnRotationChanged() {}

void Entity::OnScaleChanged() {}

void Entity::KickSelf() const {
	GameGlobal::board->RegistryForExit(m_location);
}

} // namespace Game
