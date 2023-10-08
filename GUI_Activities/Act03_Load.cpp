#include "Act03_Load.h"

#include "../Audio/BgmSFML.h"
#include "ActivityIDs.h"

namespace Activity {

Act03_Load::Act03_Load() noexcept :
	ref_carnival(nullptr) {}

void Act03_Load::start(GUI::ICarnival& carnival) {
	ref_carnival = &carnival;
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
	if (evt.type == sf::Event::Closed) {
		ref_carnival->setTransition(GUI::Transition::Exit);
		ref_carnival->cancelKeepRunning();
	}
	else if (evt.type == sf::Event::KeyPressed) {
		//m_bgm.reset();
	}
	return;
}

void Act03_Load::update(sf::RenderWindow& window, sf::Time deltaTime) {
	window.clear(sf::Color::Blue);
	window.display();
}

}
