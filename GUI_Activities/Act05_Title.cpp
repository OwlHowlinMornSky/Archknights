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

#include "../GUI/BgmSFML.h"
#include "../GUI_3D/Scene_ITitle.h"

namespace Activity {

Act05_Title::Act05_Title() :
	m_scene(nullptr) {
	return;
}

Act05_Title::~Act05_Title() noexcept {
	return;
}

bool Act05_Title::start(GUI::Window& wnd) noexcept {
	r_wnd = wnd;

	m_scene = title::getScene();
	m_scene->setup(r_wnd->getSize());

	r_wnd->setActive(true);

	Global::BGM::instance.play("res/music/m_sys_title.ogg");
	//Global::bgm->play();
	return true;
}

void Act05_Title::stop() noexcept {
	g3d::base::setActive(true);
	m_scene.reset();
	g3d::base::setActive(false);
	r_wnd->setActive(true);
	r_wnd();
}

bool Act05_Title::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		r_wnd->setWaitingForStop();
		return 1;
	case sf::Event::Resized:
		m_scene->resize({ evt.size.width, evt.size.height });
		break;
	}
	return 0;
}

void Act05_Title::update(sf::Time dtime) {
	r_wnd->setActive(false);

	m_scene->update(dtime.asSeconds());
	m_scene->render();

	r_wnd->setActive(true);
	r_wnd->clear();
	r_wnd->draw(*m_scene);
	r_wnd->display();
	return;
}

void Act05_Title::OnEnterSysloop() noexcept {}

void Act05_Title::OnExitSysloop() noexcept {}

} // namespace Activity
