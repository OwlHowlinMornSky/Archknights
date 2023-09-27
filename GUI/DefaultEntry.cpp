
#include "DefaultEntry.h"

namespace GUI {

DefaultEntry::DefaultEntry() :
	ref_carnival(nullptr)
{}

DefaultEntry::~DefaultEntry() {}

void DefaultEntry::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		ref_carnival->cancelKeepRunning();
		break;
	default:
		break;
	}
	return;
}

void DefaultEntry::update(float dt) {
	m_shape.rotate(dt * 90.0f);

	ref_carnival->getRenderWindow().clear();
	ref_carnival->getRenderWindow().draw(m_shape);
	ref_carnival->getRenderWindow().display();
	return;
}

void DefaultEntry::start(ICarnival& carnival) {
	carnival.getRenderWindow().setFramerateLimit(60);
	ref_carnival = &carnival;
	m_shape.setFillColor(sf::Color::Red);
	m_shape.setSize({ 100.0f, 100.0f });
	m_shape.setPosition({ 400.0f, 300.0f });
	return;
}

void DefaultEntry::stop() {}

void DefaultEntry::pause() {}

void DefaultEntry::resume() {}

} // namespace GUI
