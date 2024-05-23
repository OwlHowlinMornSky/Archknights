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
#include "Act05_Title.h"

#include "../Scenes/Scene_ITitle.h"
#include <MysteryEngine/G3D/G3dGlobal.h>
#include <MysteryEngine/Client/GlobalBGM.h>

#define ST_IN     (1)
#define ST_NORMAL (2)
#define ST_LINK   (3)
#define ST_OUT    (4)
#define ST_OVER   (5)

namespace Activity {

Act05_Title::Act05_Title() :
	m_st(0),
	m_scene(nullptr) {
	return;
}

Act05_Title::~Act05_Title() noexcept {
	return;
}

bool Act05_Title::prepare(ME::Window& wnd) noexcept {
	r_wnd = wnd;

	m_titleTex.loadFromFile("assets/textures/title.png");
	m_titleTex.setSmooth(true);
	return true;
}

void Act05_Title::start() noexcept {
	m_scene = Scene::Factory::GetTitle();
	m_scene->setup(r_wnd->getSize());

	m_titleSp.setTexture(m_titleTex, true);
	m_titleSp.setOrigin(m_titleTex.getSize().x / 2.0f, m_titleTex.getSize().y / 2.0f);

	//r_wnd->setActive(true);

	updateSize();

	ME::GlobalBGM::play("res/music/m_sys_title_h.ogg");
	return;
}

void Act05_Title::stop() noexcept {
	//ME::G3dGlobal::setActive(true);
	m_scene.reset();
	//ME::G3dGlobal::setActive(false);
	//r_wnd->setActive(true);
	r_wnd();
}

bool Act05_Title::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		r_wnd->setWaitingForStop();
		return 1;
	case sf::Event::MouseButtonPressed:
		if (m_st == ST_NORMAL) {
			m_st = ST_LINK;
			m_timer = sf::Time::Zero;
		}
		break;
	case sf::Event::Resized:
		updateSize();
		break;
	}
	return 0;
}

void Act05_Title::update(sf::Time dtime) {
	m_timer += dtime;
	switch (m_st) {
	case ST_IN:
		if (m_timer > sf::milliseconds(255)) {
			m_st = ST_NORMAL;
		}
		else {
			m_screen.setFillColor(sf::Color(0, 0, 0, 255 - m_timer.asMilliseconds()));
			r_wnd->setActive(false);
			m_scene->update(dtime);
			m_scene->render();
			r_wnd->setActive(true);
			r_wnd->clear();
			r_wnd->draw(*m_scene);
			r_wnd->draw(m_titleSp);
			r_wnd->draw(m_screen);
			r_wnd->display();
			break;
		}
		[[fallthrough]];
	case ST_NORMAL:
		r_wnd->setActive(false);
		m_scene->update(dtime);
		m_scene->render();
		r_wnd->setActive(true);
		r_wnd->clear();
		r_wnd->draw(*m_scene);
		r_wnd->draw(m_titleSp);
		r_wnd->display();
		break;
	case ST_LINK:
		if (m_timer > sf::milliseconds(2550)) {
			m_st = ST_NORMAL;
		}
		else {
			m_scene->SetScale(1.0f);
			m_scene->SetOffset(-0.5f);

			r_wnd->setActive(false);
			m_scene->update(dtime);
			m_scene->render();
			r_wnd->setActive(true);
			r_wnd->clear();
			r_wnd->draw(*m_scene);
			r_wnd->draw(m_titleSp);
			r_wnd->draw(m_screen);
			r_wnd->display();
			break;
		}
		break;
	case ST_OUT:
		break;
	case ST_OVER:
		break;
	default:
		m_st = ST_IN;
		m_screen.setFillColor(sf::Color::Black);
		m_scene->SetScale(0.75f);
		m_scene->SetOffset(0.5f);
		m_timer = sf::Time::Zero;
	}
	return;
}

void Act05_Title::OnEnterSysloop() noexcept {}

void Act05_Title::OnExitSysloop() noexcept {}

void Act05_Title::updateSize() {
	auto& view = r_wnd->getView();
	sf::Vector2f size = view.getSize();

	m_scene->resize(r_wnd->getSize());
	m_screen.setSize(size);

	float rate;

	rate = size.y / m_titleTex.getSize().y * 0.4f;
	m_titleSp.setScale(rate, rate);
	m_titleSp.setPosition(size.x * 0.5f, size.y * 0.35f);

}

} // namespace Activity
