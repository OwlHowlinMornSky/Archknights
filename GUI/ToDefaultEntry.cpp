
#include "DefaultEntry.h"

#include "ToDefaultEntry.h"

namespace GUI {

std::unique_ptr<IActivity> createDefaultEntry() {
	return std::make_unique<DefaultEntry>();
}

}
