#include "ActivityDependent.h"

namespace GUI {

bool ActivityDependent::isIndependent() const {
	return false;
}

void ActivityDependent::runIndependently() {}

}
