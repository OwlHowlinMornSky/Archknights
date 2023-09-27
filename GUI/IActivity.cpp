#include "IActivity.h"

namespace GUI {

GUI::IActivity::IActivity() {}

IActivity::~IActivity() {}

bool IActivity::isIndependent() const {
	return false;
}

void IActivity::runIndependently() {}

void IActivity::handleEvent(const sf::Event& evt) {}

void IActivity::update(float dt) {}

bool IActivity::start(ICarnival& carnival) {
	return true;
}

void IActivity::stop() {}

void IActivity::pause() {}

void IActivity::resume() {}

} // namespace GUI
