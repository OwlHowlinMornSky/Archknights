#pragma once

#include <memory>

namespace GUI {

std::unique_ptr<IActivity> createDefaultEntry();
#ifdef _DEBUG
std::unique_ptr<IActivity> createTestActivity(size_t n);
#endif

}
