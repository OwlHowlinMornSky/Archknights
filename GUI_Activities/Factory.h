#pragma once

#include "../GUI/IActivity.h"
#include <memory>

namespace Activity::Factory {

std::unique_ptr<GUI::IActivity> CreateActivity(uint32_t id) noexcept;

}
