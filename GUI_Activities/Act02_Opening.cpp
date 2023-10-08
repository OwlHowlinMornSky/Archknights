#include "Act02_Opening.h"

#include <SFML/Graphics.hpp>

#include "ActivityIDs.h"

#include <assert.h>

#define ST_PIC0_IN   (1)
#define ST_PIC0_KEEP (2)
#define ST_PIC1_IN   (3)
#define ST_PIC1_KEEP (4)
#define ST_PIC1_OUT  (5)
#define ST_PIC2_IN   (6)
#define ST_PIC2_KEEP (7)
#define ST_PIC2_OUT  (8)

namespace Activity {

Act02_Opening::Act02_Opening() noexcept :
	m_status(0),
	ref_carnival(nullptr) {}

void Act02_Opening::start(GUI::ICarnival& carnival) {
	ref_carnival = &carnival;
	m_status = 0;
	m_timer = sf::Time::Zero;

	circle[0].setRadius(100.0f);
	circle[0].setOrigin(100.0f, 100.0f);
	circle[0].setPosition(500.0f, 300.0f);
	circle[1].setRadius(100.0f);
	circle[1].setOrigin(100.0f, 100.0f);
	circle[1].setPosition(700.0f, 300.0f);
	circle[2].setRadius(100.0f);
	circle[2].setOrigin(100.0f, 100.0f);
	circle[2].setPosition(600.0f, 300.0f);
	return;
}

void Act02_Opening::stop() noexcept {
	m_timer = sf::Time::Zero;
	m_status = 0;
	ref_carnival = nullptr;
	return;
}

void Act02_Opening::pause() noexcept {
	assert(false);
}

void Act02_Opening::resume() noexcept {
	assert(false);
}

uint32_t Act02_Opening::getID() noexcept {
	return IDs::ID_Opening;
}

void Act02_Opening::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		ref_carnival->setTransition(GUI::Transition::Exit);
		ref_carnival->cancelKeepRunning();
		break;
	default:
		break;
	}
	return;
}

void Act02_Opening::update(sf::RenderWindow& window, sf::Time deltaTime) {
	m_timer += deltaTime;
	window.clear();
	switch (m_status) {
	case ST_PIC0_IN:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_PIC0_KEEP;
			circle[0].setFillColor(sf::Color(255, 0, 0));
		}
		else {
			circle[0].setFillColor(sf::Color(255 * m_timer.asMilliseconds() / 250, 0, 0));
		}
		window.draw(circle[0]);
		break;
	case ST_PIC0_KEEP:
		if (m_timer >= sf::milliseconds(1000)) {
			m_timer -= sf::milliseconds(1000);
			m_status = ST_PIC1_IN;
			circle[0].setFillColor(sf::Color::Transparent);
		}
		window.draw(circle[0]);
		break;
	case ST_PIC1_IN:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_PIC1_KEEP;
			circle[1].setFillColor(sf::Color(0, 255, 0));
		}
		else {
			circle[1].setFillColor(sf::Color(0, 255 * m_timer.asMilliseconds() / 250, 0));
		}
		window.draw(circle[1]);
		break;
	case ST_PIC1_KEEP:
		if (m_timer >= sf::milliseconds(1000)) {
			m_timer -= sf::milliseconds(1000);
			m_status = ST_PIC1_OUT;
		}
		window.draw(circle[1]);
		break;
	case ST_PIC1_OUT:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_PIC2_IN;
			circle[1].setFillColor(sf::Color::Transparent);
		}
		else {
			circle[1].setFillColor(sf::Color(0, 255 - 255 * m_timer.asMilliseconds() / 250, 0));
		}
		window.draw(circle[1]);
		break;
	case ST_PIC2_IN:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_PIC2_KEEP;
			circle[2].setFillColor(sf::Color(0, 0, 255));
		}
		else {
			circle[2].setFillColor(sf::Color(0, 0, 255 * m_timer.asMilliseconds() / 250));
		}
		window.draw(circle[2]);
		break;
	case ST_PIC2_KEEP:
		if (m_timer >= sf::milliseconds(1750)) {
			m_timer -= sf::milliseconds(1750);
			m_status = ST_PIC2_OUT;
		}
		window.draw(circle[2]);
		break;
	case ST_PIC2_OUT:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = 0;
			ref_carnival->setTransition(GUI::Transition::Switch, IDs::ID_Load);
			ref_carnival->cancelKeepRunning();
			circle[2].setFillColor(sf::Color::Transparent);
		}
		else {
			circle[2].setFillColor(sf::Color(0, 0, 255 - 255 * m_timer.asMilliseconds() / 250));
		}
		window.draw(circle[2]);
		break;
	default:
		m_status = ST_PIC0_IN;
		circle[0].setFillColor(sf::Color::Transparent);
		circle[1].setFillColor(sf::Color::Transparent);
		circle[2].setFillColor(sf::Color::Transparent);
		break;
	}
	return window.display();
}

}
