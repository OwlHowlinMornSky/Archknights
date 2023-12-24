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

#include <glm/vec2.hpp>

namespace game {

class Entity {
public:
	Entity();
	virtual ~Entity() = default;

public:
	virtual void OnJoined(size_t id, size_t location);
	virtual void OnUpdate(float dt);
	virtual void OnKicking();

public:
	size_t getID() const {
		return m_id;
	}
	size_t getLocation() const {
		return m_location;
	}

	bool isUpdatable() const {
		return m_updatable;
	}

	glm::vec2 getPosition() const {
		return m_position;
	}
	void setPosition(glm::vec2 pos) {
		m_position = pos;
		OnPositionChanged();
	}

	glm::vec2 getScale() const {
		return m_scale;
	}
	void setScale(glm::vec2 scl) {
		m_scale = scl;
		OnScaleChanged();
	}

	float getRotation() const {
		return m_rotation;
	}
	void setRotation(float rot) {
		m_rotation = rot;
		OnRotationChanged();
	}

protected:
	virtual void OnPositionChanged();
	virtual void OnRotationChanged();
	virtual void OnScaleChanged();

	void KickSelf() const;

protected:
	bool m_updatable;
	float m_rotation;
	glm::vec2 m_position;
	glm::vec2 m_scale;
	size_t m_id;
	size_t m_location;
};

}
