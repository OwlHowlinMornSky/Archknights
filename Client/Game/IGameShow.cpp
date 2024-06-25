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

#include <MysteryEngine/G3D/Camera.Perspective.h>

namespace Game {

IGameShow::IGameShow() {
	auto cam = std::make_shared<ME::PerspectiveCamera>();
	cam->setFOV(45.0f);
	cam->setAspectRatio(16.0f / 9.0f);
	cam->setZNear(0.25f);
	cam->setZFar(128.0f);
	SetCamera(cam);
}

void IGameShow::SetSize(sf::Vector2u size) {
	m_rtex.create(size.x, size.y, sf::ContextSettings(24u));
	m_sp.setTexture(m_rtex.getTexture(), true);

	switch (m_camera->getType()) {
	case ME::Camera::Type::Perspective:
		((ME::PerspectiveCamera*)m_camera.get())->setAspectRatio(1.0f * size.x / size.y);
		break;
	default:
		break;
	}
}

void IGameShow::SetCamera(std::shared_ptr<ME::Camera> cam) {
	m_camera = cam;
}

std::shared_ptr<ME::Camera> IGameShow::GetCamera() const {
	return m_camera;
}

void IGameShow::AddModel(std::shared_ptr<ME::IModel> a) {
	m_anims.push_back(a);
}

void IGameShow::Update(float dt) {
	for (auto& i : m_anims) {
		i->Update(dt);
	}
}

void IGameShow::Draw() {
	ME::G3dGlobal::setActive(true);
	m_rtex.setActive(true);

	//glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	glCheck(glViewport(0, 0, m_rtex.getSize().x, m_rtex.getSize().y));

	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
	//glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	glCheck(glEnable(GL_DEPTH_TEST));
	glCheck(glDepthMask(GL_TRUE));

	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glCullFace(GL_BACK));

	Render();

	m_rtex.display();

	m_rtex.setActive(false);
	ME::G3dGlobal::setActive(false);
}

void IGameShow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	return target.draw(m_sp, states);
}

} // namespace Game
