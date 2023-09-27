#include "TestActivity.h"


TestActivity::TestActivity(size_t n) :
	m_id(n),
	ref_carnival(nullptr) {
	printf_s("Construct %zu\n", n);
}

TestActivity::~TestActivity() {
	printf_s("Destruct %zu\n", m_id);
}

void TestActivity::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		ref_carnival->setTransition(GUI::ICarnival::Exit);
		ref_carnival->cancelKeepRunning();
		break;
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::Space:
			ref_carnival->setTransition(GUI::ICarnival::Push, m_id + 1);
			ref_carnival->cancelKeepRunning();
			break;
		case sf::Keyboard::Q:
			ref_carnival->setTransition(GUI::ICarnival::Pop);
			ref_carnival->cancelKeepRunning();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return;
}

void TestActivity::update(float dt) {
	m_shape.rotate(dt * 90.0f);

	ref_carnival->getRenderWindow().clear(sf::Color::Green);
	ref_carnival->getRenderWindow().draw(m_shape);
	ref_carnival->getRenderWindow().display();
	return;
}

void TestActivity::start(GUI::ICarnival& carnival) {
	carnival.getRenderWindow().setFramerateLimit(60);
	ref_carnival = &carnival;
	m_shape.setFillColor(sf::Color::Red);
	m_shape.setSize({ 100.0f, 100.0f });
	m_shape.setPosition({ 400.0f, 300.0f });
	return;
}

void TestActivity::stop() {}

void TestActivity::pause() {}

void TestActivity::resume() {}

size_t TestActivity::getID() {
	return m_id;
}
