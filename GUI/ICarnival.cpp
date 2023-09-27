
#include "IActivity.h"

#include "ICarnival.h"

namespace GUI {

ICarnival::ICarnival(sf::RenderWindow* r_window) :
	ref_window(r_window),
	m_keepRunning(false)
{}

ICarnival::~ICarnival() {
	ref_window = nullptr;
}

void ICarnival::setTransition(Transition t) {
	m_transition = t;
	return;
}

sf::RenderWindow& ICarnival::getRenderWindow() {
	return *ref_window;
}

void ICarnival::cancelKeepRunning() {
	m_keepRunning = false;
	return;
}

void ICarnival::showMessageBox(std::string_view title, std::string_view text, MBInfo info) const {}

void ICarnival::setCloseButton(bool enabled) const {}

void ICarnival::handleTransition() {
	//auto s = std::string_view("123");

	switch (m_transition) {
	case Transition::PauseMe_and_Open:
		break;
	case Transition::StopMe_and_Open:
		break;
	case Transition::Return:
		break;
	case Transition::ReturnTo:
		break;
	case Transition::ReturnTo_and_Open:
		break;
	case Transition::Exit:
		break;
	case Transition::ForceExit:
		break;
	default:
		break;
	}
}

} // namespace GUI
