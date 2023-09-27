#include "ICarnival.h"

namespace GUI {

ICarnival::ICarnival(sf::RenderWindow* r_window) :
	ref_window(r_window) {}

ICarnival::~ICarnival() {
	ref_window = nullptr;
}

sf::RenderWindow& ICarnival::getRenderWindow() {
	return *ref_window;
}

void ICarnival::showMessageBox(std::string_view title, std::string_view text, MBInfo info = MBInfo::None) const {}

void ICarnival::setCloseButton(bool enabled) const {}

} // namespace GUI
