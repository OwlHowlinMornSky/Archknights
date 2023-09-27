
#include "ICarnival.h"

namespace GUI {

ICarnival::ICarnival() {}

ICarnival::~ICarnival() {}

void ICarnival::showMessageBox(std::string_view title, std::string_view text, MBInfo info) const {}

void ICarnival::setCloseButton(bool enabled) const {}

} // namespace GUI
