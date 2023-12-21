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

#include <glm/vec3.hpp>

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

	glm::vec3 getPosition() const {
		return m_position;
	}
	void setPosition(glm::vec3 pos) {
		m_position = pos;
		OnPositionChanged();
	}

	glm::vec3 getRotation() const {
		return m_rotation;
	}
	void setRotation(glm::vec3 rot) {
		m_rotation = rot;
		OnRotationChanged();
	}

	glm::vec3 getScale() const {
		return m_scale;
	}
	void setScale(glm::vec3 scl) {
		m_scale = scl;
		OnScaleChanged();
	}

protected:
	virtual void OnPositionChanged();
	virtual void OnRotationChanged();
	virtual void OnScaleChanged();

	void KickSelf() const;

protected:
	bool m_updatable;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	size_t m_id;
	size_t m_location;
};

}
