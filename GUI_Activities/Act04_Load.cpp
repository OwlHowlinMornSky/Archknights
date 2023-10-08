/*
*    Archknights
*
*    Copyright (C) 2023  Tyler Parret True
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
#include "Act04_Load.h"

#include "../Audio/BgmSFML.h"
#include "ActivityIDs.h"

namespace Activity {

Act03_Load::Act03_Load() noexcept :
	ref_carnival(nullptr) {}

void Act03_Load::start(GUI::ICarnival& carnival) {
	ref_carnival = &carnival;

	m_tex.loadFromFile("res\\textures\\titleback.png");
	m_sp.setTexture(m_tex, true);

	updateSize(ref_carnival->getRenderWindow().getSize());

	m_bgm = std::make_unique<Audio::BgmSFML>();
	m_bgm->openFromFile("res\\music\\m_sys_title.ogg");
	//m_bgm->openFromFile("res\\music\\m_sys_title_h.ogg");
	m_bgm->play();
}

void Act03_Load::stop() noexcept {
	m_bgm->stop();
}

void Act03_Load::pause() noexcept {}

void Act03_Load::resume() noexcept {}

uint32_t Act03_Load::getID() noexcept {
	return IDs::ID_Load;
}

void Act03_Load::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		ref_carnival->setTransition(GUI::Transition::Exit);
		ref_carnival->cancelKeepRunning();
		break;
	case sf::Event::KeyPressed:
		//m_bgm.reset();
		break;
	case sf::Event::Resized:
		updateSize({ evt.size.width, evt.size.height });
		break;
	default:
		break;
	}
	return;
}

void Act03_Load::update(sf::RenderWindow& window, sf::Time deltaTime) {
	window.clear();
	window.draw(m_sp);
	window.display();
	return;
}

void Act03_Load::updateSize(sf::Vector2u newWindowSize) {
	auto tsize = m_tex.getSize();
	float sx = static_cast<float>(newWindowSize.x);
	float sy = static_cast<float>(newWindowSize.y);
	sx /= tsize.x;
	sy /= tsize.y;
	m_sp.setScale(sx, sy);
	return;
}

}
