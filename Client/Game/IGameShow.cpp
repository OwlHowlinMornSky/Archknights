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
#include <GL/glew.h>
#include <MysteryEngine/G3D/GlCheck.h>
#include <MysteryEngine/G3D/G3dGlobal.h>

#include "IGameShow.h"

namespace Game {

IGameShow::IGameShow() {
	m_camera.setFOV(45.0f);
	m_camera.setAspectRatio(16.0f / 9.0f);
	m_camera.setZNear(0.25f);
	m_camera.setZFar(128.0f);
}

void IGameShow::AddModel(std::shared_ptr<ME::IModel> a) {
	m_anims.push_back(a);
}

void IGameShow::update(float dt) {
	for (auto& i : m_anims) {
		i->Update(dt);
	}
}

void IGameShow::onSizeChanged(sf::Vector2u newsize) {
	switch (m_camera.getType()) {
	case ME::Camera::Type::Perspective:
		m_camera.setAspectRatio(1.0f * newsize.x / newsize.y);
		break;
	default:
		break;
	}
}

} // namespace Game
