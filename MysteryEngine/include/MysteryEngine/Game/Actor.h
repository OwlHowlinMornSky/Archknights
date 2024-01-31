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
#pragma once

#include <MysteryEngine/Core/GLM.h>

namespace ME {

class Actor {
public:
	enum class Type {
		Animation = 0,
		Model,
		COUNT
	};
	Actor() = default;

	virtual glm::vec3 getPosition() const = 0;
	virtual void setPosition(glm::vec3 pos) = 0;
	virtual glm::vec3 getRotation() const = 0;
	virtual void setRotation(glm::vec3 rot) = 0;
	virtual glm::vec3 getScale() const = 0;
	virtual void setScale(glm::vec3 scl) = 0;

	bool IsWaitingForQuit() const {
		return m_waitingForQuit;
	}

protected:
	bool m_waitingForQuit;
	ME::Actor::Type m_type;
};

} // namespace game
