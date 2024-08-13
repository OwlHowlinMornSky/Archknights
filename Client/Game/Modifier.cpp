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
	m_target() {}

Game::ModifySwitch::~ModifySwitch() {
	setEnabled(false);
}

void Game::ModifySwitch::setSource(std::weak_ptr<Entity> _e, Modifier::SourceType::EntityFunction _f) {
	m_modifier.source.entity = _e;
	m_modifier.source.func = _f;
	if (m_enabled) {
		(*m_handle) = m_modifier;
		auto target = m_target.lock();
		assert(target != nullptr);
		target->onModifierChanged(m_attribute);
	}
}

void Game::ModifySwitch::setTarget(std::weak_ptr<Entity> target, Entity::AttributeType attrib) {
	setEnabled(false);
	m_target = target;
	m_attribute = attrib;
}

void Game::ModifySwitch::setValue(Attribute::ValueType grow, Attribute::ValueType percent, Attribute::ValueType add, Attribute::ValueType times) {
	m_modifier.value[Modifier::ModifyType::Grow] = grow;
	m_modifier.value[Modifier::ModifyType::Percent] = percent;
	m_modifier.value[Modifier::ModifyType::Add] = add;
	m_modifier.value[Modifier::ModifyType::Times] = times;
	if (m_enabled) {
		(*m_handle) = m_modifier;
		auto target = m_target.lock();
		assert(target != nullptr);
		target->onModifierChanged(m_attribute);
	}
}

bool Game::ModifySwitch::setEnabled(bool enabled) {
	if (enabled == m_enabled) {
		return true;
	}
	if (enabled) {
		if (m_target.expired() || m_attribute >= Entity::AttributeType::COUNT) {
			m_enabled = false;
			return false;
		}
		auto target = m_target.lock();
		assert(target != nullptr);
		m_handle = target->setModifier(m_attribute, m_modifier);
		m_enabled = true;
	}
	else {
		auto target = m_target.lock();
		assert(target != nullptr);
		target->eraseModifier(m_attribute, m_handle);
		m_enabled = false;
	}
	return true;
}

bool Game::ModifySwitch::getEnabled() const {
	return m_enabled;
}
