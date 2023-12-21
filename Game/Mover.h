#pragma once

#include "Unit.h"
#include "GameBoard.h"

namespace game {

class Mover :
	public Unit {

protected:
	b2Body* m_blockTarget;
	b2Body* m_searchTarget;

	static b2FixtureDef default_block_target;
};

}
