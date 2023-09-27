
#include "DefaultEntry.h"

#include "ToDefaultEntry.h"

#include "TestActivity.h"

namespace GUI {

std::unique_ptr<IActivity> createDefaultEntry() {
	return std::make_unique<DefaultEntry>();
}

std::unique_ptr<IActivity> createTestActivity(size_t n) {
    return std::make_unique<TestActivity>(n);
}

}
