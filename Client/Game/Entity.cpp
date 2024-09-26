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
#include "Global.h"
#include "Board.h"
#include "MsgResult.h"

#include "BasicMsgId.h"
#include "AtkDmgHeal.h"

#include <iostream>

namespace Game {

Entity::Entity() :
	m_position{ 0.0f, 0.0f },
	m_rotation(0.0f),
	m_scale{ 1.0f, 1.0f },

	m_isNotWaitingForExit(true),

	m_myself(),

	m_hp(256.0f)

{}

void Entity::basicOnJoined(EntityIdType id, std::weak_ptr<Entity> yourself) {
	m_id = id;
	m_myself = yourself;

	m_attributes[AttributeType::MaxHp].original = 256.0f;
	m_attributes[AttributeType::MaxHp].effective = 256.0f;

	// for test
	std::cout << "Joined. Id: " << id << "Addr: 0x" << this << "." << std::endl;
	return onJoined();
}

void Entity::basicOnKicking() {
	onKicking();
	std::cout << "Kicked. Id: " << m_id << "Addr: 0x" << this << "." << std::endl;
	m_myself.reset();
	m_id = 0;
	return;
}

void Entity::onJoined() {}

void Entity::onKicking() {}

MsgResultType Entity::EntityProc(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	EntityMsg Msg{ msg, wparam, lparam };
	auto i = m_hooks.begin(), n = m_hooks.end();
	while (i != n) {
		switch ((*i)->callHook(*this, Msg)) {
		case 1:
		{
			std::list<std::shared_ptr<Hook>>::iterator j = i;
			++i;
			m_hooks.erase(j);
			break;
		}
		case 2:
			i = n;
			break;
		default:
			++i;
			break;
		}
	}
	return receiveMessage(msg, wparam, lparam);
}

MsgResultType Entity::receiveMessage(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	return DefEntityProc(msg, wparam, lparam);
}

std::list<Modifier>::iterator Entity::setModifier(AttributeType attribute, Modifier& data) {
	m_modifiers[attribute].push_back(data);
	std::list<Modifier>::iterator res = m_modifiers[attribute].end();
	res--;
	onModifierChanged(attribute);
	return res;
}

void Entity::eraseModifier(AttributeType attribute, std::list<Modifier>::iterator iterator) {
	m_modifiers[attribute].erase(iterator);
	onModifierChanged(attribute);
	return;
}

void Entity::onModifierChanged(AttributeType attribute) {
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
	Attribute::ValueType res = m_attributes[attribute].original;
	res = ((res + grow) * percent + add) * times;
	if (res > m_attributes[attribute].max)
		res = m_attributes[attribute].max;
	else if (res < m_attributes[attribute].min)
		res = m_attributes[attribute].min;
	m_attributes[attribute].effective = res;
	return;
}

void Entity::changeAbility(AbilityType type, Ability::ValueType val) {
	m_abilities[type].effective += val;
}

Ability::ValueType Entity::getAbility(AbilityType type) const {
	return m_abilities[type].effective;
}

Attribute::ValueType Entity::getHp() const {
	return m_hp;
}

Attribute::ValueType Entity::getAttribute(AttributeType type) const {
	return m_attributes[type].effective;
}

bool Entity::fixedUpdate() {
	return m_isNotWaitingForExit;
}

EntityIdType Entity::getID() const {
	return m_id;
}

const float* Entity::getPosition() const {
	return m_position;
}

void Entity::setPosition(float x, float y) {
	m_position[0] = x;
	m_position[1] = y;
	onPositionChanged();
}

const float* Entity::getScale() const {
	return m_scale;
}

void Entity::setScale(float x, float y) {
	m_scale[0] = x;
	m_scale[1] = y;
	onScaleChanged();
}

float Entity::getRotation() const {
	return m_rotation;
}

void Entity::setRotation(float rot) {
	m_rotation = rot;
	onRotationChanged();
}

void Entity::onPositionChanged() {}

void Entity::onRotationChanged() {}

void Entity::onScaleChanged() {}

void Entity::kickSelf() {
	m_isNotWaitingForExit = false;
}

MsgResultType Entity::DefEntityProc(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	switch (msg) {
	case Game::MsgId::OnGetAttack:
		receiveMessage(Game::MsgId::OnGetDamage, 0, (intptr_t)(&(((AttackData*)lparam)->damage)));
		break;
	case Game::MsgId::OnGetHeal:
	{
		HealData* data = (HealData*)lparam;
		if (!data)
			break;
		if (data->healValue < 0.0f)
			data->healValue = 0.0f;
		m_hp += data->healValue;
		receiveMessage(Game::MsgId::OnHpChanging, 0, 0);
		break;
	}
	case Game::MsgId::OnGetDamage:
	{
		DamageData* data = (DamageData*)lparam;
		if (!data)
			break;
		Attribute::ValueType dmg = 0.0f;
		switch (data->type) {
		case DamageData::Normal:
			dmg = data->dmgValue - m_attributes[AttributeType::Def].effective;
			break;
		case DamageData::Magical:
			dmg = data->dmgValue * (1.0f - m_attributes[AttributeType::MagDef].effective);
			break;
		case DamageData::Direct:
		default:
			dmg = data->dmgValue;
			break;
		}
		if (data->minValue < 0.0f)
			data->minValue = 0.0f;
		else if (data->minValue > 1.0f)
			data->minValue = 1.0f;
		if (dmg < data->dmgValue * data->minValue)
			dmg = data->dmgValue * data->minValue;
		if (dmg < 0.0f)
			dmg = 0.0f;
		m_hp -= dmg;
		receiveMessage(Game::MsgId::OnHpChanging, 0, 0);
		break;
	}
	case Game::MsgId::OnHpChanging:
		if (m_hp <= 0.0f)
			m_hp = 0.0f;
		else if (m_hp > m_attributes[AttributeType::MaxHp].effective)
			m_hp = m_attributes[AttributeType::MaxHp].effective;
		receiveMessage(Game::MsgId::OnHpChanged, 0, 0);
		break;
	case Game::MsgId::OnHpChanged:
		if (m_hp <= 0.0f)
			receiveMessage(Game::MsgId::OnHpDropToZero, 0, 0);
		break;
	case Game::MsgId::OnHpDropToZero:
		kickSelf();
		break;
	default:
		return MsgResult::Unsubscribe;
	}
	return MsgResult::OK;
}

void Entity::setAttributeOringalValue(AttributeType type, Attribute::ValueType val) {
	m_attributes[type].original = val;
	return onModifierChanged(type);
}

} // namespace Game
