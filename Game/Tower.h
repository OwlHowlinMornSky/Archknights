#pragma once

#include "Unit.h"
#include "GameBoard.h"

namespace game {

class Tower :
	public Unit {

protected:
	b2Body* m_blockDetector;
	b2Body* m_searchTarget;
};

}
