#include "ActivityIndependent.h"

namespace GUI {

bool ActivityIndependent::isIndependent() const {
	return true;
}

void ActivityIndependent::handleEvent(const sf::Event& evt) {}

void ActivityIndependent::update(float dt) {}

} // namespace GUI
