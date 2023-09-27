
#include "DefaultEntry.h"

namespace GUI {

DefaultEntry::DefaultEntry() :
	ref_carnival(nullptr) {
	printf_s("Construct 1\n");
}

DefaultEntry::~DefaultEntry() {
	printf_s("Destruct 1\n");
}

void DefaultEntry::handleEvent(const sf::Event& evt) {
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

void DefaultEntry::update(float dt) {
	ref_carnival->getRenderWindow().clear(sf::Color::Red);
	ref_carnival->getRenderWindow().display();
	return;
}

void DefaultEntry::start(ICarnival& carnival) {
	carnival.getRenderWindow().setFramerateLimit(60);
	ref_carnival = &carnival;
	return;
}

void DefaultEntry::stop() {}

void DefaultEntry::pause() {}

void DefaultEntry::resume() {}

size_t DefaultEntry::getID() {
	return 1ull;
}

} // namespace GUI
