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

void IGameShow::AddAnimation(std::shared_ptr<ME::IModel> a) {
	m_anims.push_back(a);
}

void IGameShow::Update(float dt) {
	UpdateModels(dt);

	ME::G3dGlobal::setActive(true);
	m_rtex.setActive(true);
	glCheck(glClear(GL_COLOR_BUFFER_BIT));
	glCheck(glViewport(0, 0, m_rtex.getSize().x, m_rtex.getSize().y));

	Render();

	m_rtex.display();

	m_rtex.setActive(false);
	ME::G3dGlobal::setActive(false);
}

void IGameShow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	return target.draw(m_sp, states);
}

} // namespace Game
