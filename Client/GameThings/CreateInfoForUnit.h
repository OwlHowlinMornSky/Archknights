#pragma once

#include <memory>
#include "IActor.h"

namespace Game {

struct CreateInfoForUnit {
	std::shared_ptr<Game::IActor> actor;
};

}
