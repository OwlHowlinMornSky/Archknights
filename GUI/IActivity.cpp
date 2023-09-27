
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

} // namespace GUI
