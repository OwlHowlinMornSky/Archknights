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

#include "BasicMsgId.h"

#include <iostream>

namespace Game {

Entity::Entity() :
	m_position{ 0.0f, 0.0f },
	m_rotation(0.0f),
	m_scale{ 1.0f, 1.0f },

	m_id(0),
	m_location(0),

	m_hp(256.0f)

{}

void Entity::BasicOnJoined(EntityIdType id, EntityLocationType location) {
	m_id = id;
	m_location = location;

	attributes[AttributeType::MaxHp].original = 256.0f;
	attributes[AttributeType::MaxHp].effective = 256.0f;

	// for test
	std::cout << "Join: " << this << ", ID: " << id << ", Location: " << location << std::endl;
	return OnJoined();
}

void Entity::BasicOnKicking() {
	OnKicking();
	std::cout << "Kick: " << this << ", ID: " << m_id << ", Location: " << m_location << std::endl;
	m_location = 0;
	m_id = 0;
	return;
}

void Entity::OnJoined() {}

void Entity::OnKicking() {}

MsgResultType Entity::ReceiveMessage(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	return MsgResult::Unsubscribe;
}

std::list<Modifier>::iterator Entity::Modify(AttributeType attribute, Modifier& data) {
	m_modifiers[attribute].push_back(data);
	std::list<Modifier>::iterator res = m_modifiers[attribute].end();
	res--;
	OnModifierChanged(attribute);
	return res;
}

void Entity::ModifyRemove(AttributeType attribute, std::list<Modifier>::iterator iterator) {
	m_modifiers[attribute].erase(iterator);
	OnModifierChanged(attribute);
	return;
}

void Entity::OnModifierChanged(AttributeType attribute) {
	Attribute::ValueType grow = 0, percent = 1, add = 0, times = 1;
	for (auto& it : m_modifiers[attribute]) {
		grow += it.value[Modifier::ModifyType::Grow];
		percent += it.value[Modifier::ModifyType::Percent];
		add += it.value[Modifier::ModifyType::Add];
		auto t = it.value[Modifier::ModifyType::Times];
		if (t <= 0)
			t += 1;
		times *= t;
	}
	if (percent < 0)
		percent = 0;
	Attribute::ValueType res = attributes[attribute].original;
	res = ((res + grow) * percent + add) * times;
	if (res > attributes[attribute].max)
		res = attributes[attribute].max;
	else if (res < attributes[attribute].min)
		res = attributes[attribute].min;
	attributes[attribute].effective = res;
	return;
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

MsgResultType Entity::DefEntityProc(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	switch (msg) {
	case Game::MsgId::OnGetAttack:
		ReceiveMessage(Game::MsgId::OnGetDamage, wparam, 0);
		break;
	case Game::MsgId::OnGetDamage:
		m_hp -= wparam;
		ReceiveMessage(Game::MsgId::OnHpChanged, wparam, 0);
		break;
	case Game::MsgId::OnHpChanged:
		if (m_hp <= 0.0f) {
			m_hp = 0;
			ReceiveMessage(Game::MsgId::OnHpDropToZero, wparam, 0);
		}
		else if (m_hp > attributes[AttributeType::MaxHp].effective) {
			m_hp = attributes[AttributeType::MaxHp].effective;
		}
		break;
	case Game::MsgId::OnHpDropToZero:
		KickSelf();
		break;
	default:
		return MsgResult::Unsubscribe;
	}
	return MsgResult::OK;
}

} // namespace Game
