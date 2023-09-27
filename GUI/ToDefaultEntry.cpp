
#include "DefaultEntry.h"

#include "ToDefaultEntry.h"

#include "TestActivity.h"

namespace GUI {

std::unique_ptr<IActivity> createDefaultEntry() {
#ifdef _DEBUG
	return std::make_unique<DefaultEntryDebug>();
#else
	return std::make_unique<DefaultEntry>();
#endif
}

#ifdef _DEBUG
std::unique_ptr<IActivity> createTestActivity(size_t n) {
    return std::make_unique<TestActivity>(n);
}
#endif

}
