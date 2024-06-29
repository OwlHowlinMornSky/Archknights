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
#include "Modifier.h"

#include "Global.h"
#include "Board.h"

#include <cassert>

Game::ModifySwitch::ModifySwitch() :
	m_enabled(false),
	m_attribute(Game::Entity::AttributeType::COUNT),
	m_targetAd(0),
	m_targetId(0) {}

Game::ModifySwitch::~ModifySwitch() {
	SetEnabled(false);
}

void Game::ModifySwitch::SetSource(EntityLocationType _ad, EntityIdType _id, Modifier::SourceType::EntityFunction _f) {
	m_modifier.source.ad = _ad;
	m_modifier.source.id = _id;
	m_modifier.source.func = _f;
	if (m_enabled) {
		(*m_handle) = m_modifier;
		auto target = GameGlobal::board->EntityAt(m_targetAd);
		assert(target != nullptr);
		assert(target->getID() == m_targetId);
		target->OnModifierChanged(m_attribute);
	}
}

void Game::ModifySwitch::SetTarget(EntityLocationType targetAd, EntityIdType targetId, Entity::AttributeType attrib) {
	SetEnabled(false);
	m_targetAd = targetAd;
	m_targetId = targetId;
	m_attribute = attrib;
}

void Game::ModifySwitch::SetValue(Attribute::ValueType grow, Attribute::ValueType percent, Attribute::ValueType add, Attribute::ValueType times) {
	m_modifier.value[Modifier::ModifyType::Grow] = grow;
	m_modifier.value[Modifier::ModifyType::Percent] = percent;
	m_modifier.value[Modifier::ModifyType::Add] = add;
	m_modifier.value[Modifier::ModifyType::Times] = times;
	if (m_enabled) {
		(*m_handle) = m_modifier;
		auto target = GameGlobal::board->EntityAt(m_targetAd);
		assert(target != nullptr);
		assert(target->getID() == m_targetId);
		target->OnModifierChanged(m_attribute);
	}
}

bool Game::ModifySwitch::SetEnabled(bool enabled) {
	if (enabled == m_enabled) {
		return true;
	}
	if (enabled) {
		if (m_targetId == 0 || m_targetAd == 0 || m_attribute >= Entity::AttributeType::COUNT) {
			m_enabled = false;
			return false;
		}
		auto target = GameGlobal::board->EntityAt(m_targetAd);
		assert(target != nullptr);
		assert(target->getID() == m_targetId);
		m_handle = target->Modify(m_attribute, m_modifier);
		m_enabled = true;
	}
	else {
		auto target = GameGlobal::board->EntityAt(m_targetAd);
		assert(target != nullptr);
		assert(target->getID() == m_targetId);
		target->ModifyRemove(m_attribute, m_handle);
		m_enabled = false;
	}
	return true;
}

bool Game::ModifySwitch::GetEnabled() const {
	return m_enabled;
}
