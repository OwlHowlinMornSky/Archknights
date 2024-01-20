#pragma once

#include "IGamePhysics.h"
#include <box2d/box2d.h>

namespace game {

class GamePhysics :
	public IGamePhysics {
public:
	GamePhysics();
	virtual ~GamePhysics() override = default;

protected:
	b2World m_world;
};

}

