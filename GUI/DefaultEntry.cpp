
#include "DefaultEntry.h"

#include "ActivityIDs.h"

namespace GUI {

#ifdef _DEBUG
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
			ref_carnival->setTransition(ICarnival::Push, ID_RESERVED_MAX + 1);
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
	return GUI::ID_DefaultEntry;
}
#else
DefaultEntry::DefaultEntry() :
	m_haveRunned(false),
	ref_carnival(nullptr)
{}

DefaultEntry::~DefaultEntry() {}

bool DefaultEntry::isIndependent() const {
	return true;
}

void DefaultEntry::runIndependently() {
	if (m_haveRunned) {
		ref_carnival->setTransition(ICarnival::Exit);
		ref_carnival->cancelKeepRunning();
		return;
	}
	m_haveRunned = true;
}

void DefaultEntry::start(ICarnival& carnival) {
	ref_carnival = &carnival;
	return;
}

void DefaultEntry::stop() {
	ref_carnival = nullptr;
	return;
}

void DefaultEntry::pause() {}

void DefaultEntry::resume() {}

size_t DefaultEntry::getID() {
	return GUI::ID_DefaultEntry;
}


#endif


} // namespace GUI
