
#include "DefaultEntry.h"

namespace GUI {

DefaultEntryDebug::DefaultEntryDebug() :
	ref_carnival(nullptr) {
	printf_s("Construct 1\n");
}

DefaultEntryDebug::~DefaultEntryDebug() {
	printf_s("Destruct 1\n");
}

void DefaultEntryDebug::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		ref_carnival->setTransition(ICarnival::Exit);
		ref_carnival->cancelKeepRunning();
		break;
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::Space:
			ref_carnival->setTransition(ICarnival::Push, 2);
			ref_carnival->cancelKeepRunning();
			break;
		case sf::Keyboard::Q:
			ref_carnival->setTransition(ICarnival::Pop);
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

void DefaultEntryDebug::update(float dt) {
	ref_carnival->getRenderWindow().clear(sf::Color::Red);
	ref_carnival->getRenderWindow().display();
	return;
}

void DefaultEntryDebug::start(ICarnival& carnival) {
	carnival.getRenderWindow().setFramerateLimit(60);
	ref_carnival = &carnival;
	return;
}

void DefaultEntryDebug::stop() {}

void DefaultEntryDebug::pause() {}

void DefaultEntryDebug::resume() {}

size_t DefaultEntryDebug::getID() {
	return 1ull;
}

} // namespace GUI
