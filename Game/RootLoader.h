#pragma once

#include "Entity.h"

namespace game {

class RootLoader :
	public Entity {
	using Parent = game::Entity;
public:

// 继承的
public:
	virtual void OnJoined(size_t id, size_t location) override;
	virtual void OnUpdate(size_t dt) override;
	virtual void OnKicking() override;
};

}
