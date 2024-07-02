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
#include <GL/glew.h>
#include <MysteryEngine/G3D/Scene.h>
#include <MysteryEngine/G3D/GlCheck.h>
#include <MysteryEngine/G3D/G3dGlobal.h>

namespace ME {

void Scene::resize(sf::Vector2u size) {
	m_renderTexture.create(size.x, size.y, sf::ContextSettings(24u));
	m_sprite.setTexture(m_renderTexture.getTexture(), true);
	onSizeChanged(size);
	return;
}

Camera& Scene::getCamera() {
	return m_camera;
}

const Camera& Scene::getCamera() const {
	return m_camera;
}

void Scene::setup(int code, void* data) {
	return;
}

void Scene::clear() {
	return;
}

void Scene::update(float dt) {
	return;
}

void Scene::render() {
	ME::G3dGlobal::SetActive(true);
	m_renderTexture.setActive(true);
	onRender();
	m_renderTexture.display();
	m_renderTexture.setActive(false);
	ME::G3dGlobal::SetActive(false);
	return;
}

bool Scene::testPoint(sf::Vector2i pt, glm::vec3* outpt) {
	if (outpt == nullptr)
		return false;
	sf::Vector2u sz = m_renderTexture.getSize();

	if (pt.x < 0)
		pt.x = 0;
	else if (pt.x >= (int)sz.x)
		pt.x = sz.x - 1;
	pt.y = sz.y - pt.y;
	if (pt.y < 0)
		pt.y = 0;
	else if (pt.y >= (int)sz.y)
		pt.y = sz.y - 1;

	float d = 0.0f;
	ME::G3dGlobal::SetActive(true);
	m_renderTexture.setActive(true);
	glCheck(glReadPixels(pt.x, pt.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &d));
	GLenum errorCode = glGetError();
	if (errorCode != GL_NO_ERROR) {
		m_renderTexture.setActive(false);
		ME::G3dGlobal::SetActive(false);
		return false;
	}
	m_renderTexture.setActive(false);
	ME::G3dGlobal::SetActive(false);

	glm::vec4 ndc(pt.x * 2.0f / sz.x - 1.0f, pt.y * 2.0f / sz.y - 1.0f, d * 2.0f - 1.0f, 1.0f);

	outpt[0] = m_camera.testPointFromNdcToWorld(ndc);

	return true;
}

void Scene::testDirection(sf::Vector2i pt, glm::vec3* outDirectionNotNormalized, glm::vec3* startPt) {
	sf::Vector2u sz = m_renderTexture.getSize();
	pt.y = sz.y - pt.y;
	glm::vec4 ndc(pt.x * 2.0f / sz.x - 1.0f, pt.y * 2.0f / sz.y - 1.0f, 1.0f, 1.0f);
	glm::vec3 d, p;
	m_camera.testDirectionFromNdcToWorld(ndc, d, p);
	if (outDirectionNotNormalized != nullptr)
		outDirectionNotNormalized[0] = d;
	if (startPt != nullptr)
		startPt[0] = p;
	return;
}

void Scene::testDirectionInCameraCoord(sf::Vector2i pt, glm::vec3* outDirectionNotNormalized, glm::vec3* startPt) {
	if (outDirectionNotNormalized == nullptr)
		return;
	sf::Vector2u sz = m_renderTexture.getSize();
	pt.y = sz.y - pt.y;
	glm::vec4 ndc(pt.x * 2.0f / sz.x - 1.0f, pt.y * 2.0f / sz.y - 1.0f, 0.0f, 1.0f);
	glm::vec3 d, p;
	m_camera.testDirectionFromNdcToCamera(ndc, d, p);
	if (outDirectionNotNormalized != nullptr)
		outDirectionNotNormalized[0] = d;
	if (startPt != nullptr)
		startPt[0] = p;
	return;
}

void ME::Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	return target.draw(m_sprite, states);
}

void Scene::onRender() {
	return;
}

void Scene::onSizeChanged(sf::Vector2u newsize) {
	return;
}

} // namespace ME
