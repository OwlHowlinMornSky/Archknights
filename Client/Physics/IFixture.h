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

#include "../Game/TypeDef.h"
#include "../Game/Entity.h"

namespace Physics {

class IFixture {
	friend class Detector;
	friend class DetectorIndependent;
protected:
	IFixture() :
		m_master(false),
		m_id(0),
		m_holder() {}
public:
	virtual ~IFixture() = default;

public:
	virtual void onBeginContact(IFixture* another) = 0;
	virtual void onEndContact(IFixture* another) = 0;

	bool IsMaster() const {
		return m_master;
	}

	Game::EntityIdType getId() const {
		return m_id;
	}
	void setId(Game::EntityIdType id) {
		m_id = id;
	}
	std::weak_ptr<Game::Entity> getHolder() const {
		return m_holder;
	}
	void setHolder(std::weak_ptr<Game::Entity> holder) {
		m_holder = holder;
	}

protected:
	bool m_master;
	Game::EntityIdType m_id;
	std::weak_ptr<Game::Entity> m_holder;
};

}
