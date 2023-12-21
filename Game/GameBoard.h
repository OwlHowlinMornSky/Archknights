#pragma once

#include <deque>
#include "GameGlobal.h"

namespace game {

class Entity;

class GameBoard {



protected:
	std::deque<Entity*> m_entities;
};

}
