#pragma once

#include <memory>

namespace GUI {

std::unique_ptr<IActivity> createDefaultEntry();
std::unique_ptr<IActivity> createTestActivity(size_t n);

}
