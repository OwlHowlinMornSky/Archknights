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
#include <MysteryEngine/G3D/TitleScene.h>

#include <MysteryEngine/G3D/LineFrameModel.h>
#include <MysteryEngine/Core/RandGen.h>
#include <MysteryEngine/G3D/G3dGlobal.h>
#include <MysteryEngine/G3D/GlCheck.h>

namespace {

constexpr unsigned long Vertices[] = {
	0x3f59c440, 0, 0x3f069650,
	0x3f069650, 0x3f59c440, 0,
	0, 0x3f069650, 0x3f59c440,
	0, 0xbf069650, 0x3f59c440,
	0x3f069650, 0xbf59c440, 0,
	0x3f59c440, 0, 0xbf069650,
	0xbf59c440, 0, 0xbf069650,
	0, 0x3f069650, 0xbf59c440,
	0xbf069650, 0x3f59c440, 0,
	0xbf59c440, 0, 0x3f069650,
	0xbf069650, 0xbf59c440, 0,
	0, 0xbf069650, 0xbf59c440,
	0x3f4f1bbd, 0x3f000000, 0x3e9e3779,
	0x3f000000, 0x3e9e3779, 0x3f4f1bbd,
	0x3f000000, 0xbe9e3779, 0x3f4f1bbd,
	0x3f4f1bbd, 0xbf000000, 0x3e9e3779,
	0x3f800000, 0, 0,
	0x3e9e3779, 0x3f4f1bbd, 0x3f000000,
	0, 0, 0x3f800000,
	0x3e9e3779, 0xbf4f1bbd, 0x3f000000,
	0x3f4f1bbd, 0xbf000000, 0xbe9e3779,
	0x3f4f1bbd, 0x3f000000, 0xbe9e3779,
	0x3e9e3779, 0x3f4f1bbd, 0xbf000000,
	0xbe9e3779, 0x3f4f1bbd, 0x3f000000,
	0xbf000000, 0xbe9e3779, 0x3f4f1bbd,
	0, 0xbf800000, 0,
	0x3f000000, 0xbe9e3779, 0xbf4f1bbd,
	0, 0x3f800000, 0,
	0xbf000000, 0x3e9e3779, 0x3f4f1bbd,
	0xbe9e3779, 0xbf4f1bbd, 0x3f000000,
	0x3e9e3779, 0xbf4f1bbd, 0xbf000000,
	0x3f000000, 0x3e9e3779, 0xbf4f1bbd,
	0xbf000000, 0x3e9e3779, 0xbf4f1bbd,
	0xbf4f1bbd, 0x3f000000, 0xbe9e3779,
	0xbf800000, 0, 0,
	0xbf4f1bbd, 0xbf000000, 0xbe9e3779,
	0xbf000000, 0xbe9e3779, 0xbf4f1bbd,
	0xbe9e3779, 0x3f4f1bbd, 0xbf000000,
	0xbf4f1bbd, 0x3f000000, 0x3e9e3779,
	0xbf4f1bbd, 0xbf000000, 0x3e9e3779,
	0xbe9e3779, 0xbf4f1bbd, 0xbf000000,
	0, 0, 0xbf800000
};

constexpr int Indices[] = {
	0, 12, 12, 1, 0, 13, 13, 2, 0, 14, 14, 3, 0, 15, 15, 4, 0, 16, 16, 5, 1, 17, 17,
	2, 2, 18, 18, 3, 3, 19, 19, 4, 4, 20, 20, 5, 5, 21, 21, 1, 1, 22, 22, 7, 2, 23, 23,
	8, 3, 24, 24, 9, 4, 25, 25, 10, 5, 26, 26, 11, 8, 27, 27, 1, 9, 28, 28, 2, 10, 29,
	29, 3, 11, 30, 30, 4, 7, 31, 31, 5, 6, 32, 32, 7, 6, 33, 33, 8, 6, 34, 34, 9, 6,
	35, 35, 10, 6, 36, 36, 11, 7, 37, 37, 8, 8, 38, 38, 9, 9, 39, 39, 10, 10, 40, 40,
	11, 11, 41, 41, 7, 13, 12, 12, 17, 17, 13, 14, 13, 13, 18, 18, 14, 15, 14, 14, 19,
	19, 15, 16, 15, 15, 20, 20, 16, 12, 16, 16, 21, 21, 12, 32, 33, 33, 37, 37, 32, 33,
	34, 34, 38, 38, 33, 34, 35, 35, 39, 39, 34, 35, 36, 36, 40, 40, 35, 36, 32, 32, 41,
	41, 36, 27, 22, 22, 37, 37, 27, 28, 23, 23, 38, 38, 28, 29, 24, 24, 39, 39, 29, 30,
	25, 25, 40, 40, 30, 31, 26, 26, 41, 41, 31, 27, 23, 23, 17, 17, 27, 28, 24, 24, 18,
	18, 28, 29, 25, 25, 19, 19, 29, 30, 26, 26, 20, 20, 30, 31, 22, 22, 21, 21, 31,
};

} // namespace

namespace ME {

struct TitleScene::MEMBERS {
	LineFrameShader shader;
	LineFrameModel model;
	float rotSpeed[3];

	MEMBERS() :
		rotSpeed() {
	}
};

TitleScene::TitleScene() {
	m = new MEMBERS;
	m_camera.setType(ME::Camera::Type::Perspective);
}

TitleScene::~TitleScene() {
	delete m;
	m = nullptr;
}

void TitleScene::setScale(float r) {
	sf::Vector2u size = m_renderTexture.getSize();
	m_camera.setAspectRatio(1.0f * size.x / size.y);
}

void TitleScene::setOffset(float r) {
	m->model.setPosition(0.0f, 0.0f, 0.0f - r);
}

void TitleScene::setup(int code, void* data) {
	m_camera.setPosition(0.0f, 0.0f, 12.0f);
	m_camera.setFOV(9.0f);

	float thick = 0.0075f;

	union I2F { // 用于把bin转化为float
		unsigned long i = 0;
		float f;
	} a[3];

	std::array<glm::vec3, 42> vertices;
	for (int i = 0; i < 42; ++i) {
		a[0].i = Vertices[i * 3];
		a[1].i = Vertices[i * 3 + 1];
		a[2].i = Vertices[i * 3 + 2];
		vertices[i] = { a[0].f, a[1].f, a[2].f };
	}

	std::vector<LineFrameVertex> va;
	va.reserve(480);
	for (int i = 0; i < 120; ++i) {
		glm::vec3& v0 = vertices[Indices[(i << 1)]];
		glm::vec3& v1 = vertices[Indices[(i << 1) | 1]];

		va.emplace_back(v0, v1, glm::vec2(0.0f, thick));
		va.emplace_back(v0, v1, glm::vec2(0.0f, -thick));
		va.emplace_back(v0, v1, glm::vec2(1.0f, -thick));
		va.emplace_back(v0, v1, glm::vec2(1.0f, thick));
	}

	m->rotSpeed[0] = ME::RandGen::GetUni01() * 20.0f + 5.0f;
	m->rotSpeed[1] = ME::RandGen::GetUni01() * 20.0f + 5.0f;
	m->rotSpeed[2] = ME::RandGen::GetUni01() * 20.0f + 5.0f;

	m->model.setRotation(
		ME::RandGen::GetUni01() * 360.0f,
		ME::RandGen::GetUni01() * 360.0f,
		ME::RandGen::GetUni01() * 360.0f
	);

	ME::G3dGlobal::SetActive(true);

	m->shader.setup();

	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));

	glCheck(glClearColor(0.2f, 0.2f, 0.2f, 1.0f)); // 设置clear颜色

	ME::G3dGlobal::SetActive(false);

	m->model.LoadModelData(va);
}

void TitleScene::clear() {
	m->model.clear();
	ME::G3dGlobal::SetActive(true);
	m->shader.clear();
	ME::G3dGlobal::SetActive(false);
}

void TitleScene::update(float dt) {
	m->model.rotate(m->rotSpeed[0] * dt, m->rotSpeed[1] * dt, m->rotSpeed[2] * dt);
	m->model.normalizeRotation();

	//m->model.Update(0.0f);
}

void TitleScene::onRender() {
	glCheck(glClear(GL_COLOR_BUFFER_BIT));
	glCheck(glViewport(0, 0, m_renderTexture.getSize().x, m_renderTexture.getSize().y));

	ME::Shader::Bind(&m->shader);

	m->model.draw(&m_camera, &m->shader);

	ME::Shader::Bind(nullptr);
}

void TitleScene::onSizeChanged(sf::Vector2u newsize) {
	m_camera.setAspectRatio(1.0f * newsize.x / newsize.y);
}

}
