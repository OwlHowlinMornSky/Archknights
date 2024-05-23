#pragma once

#include "../Game/Entity.h"
#include <string_view>
#include <memory>

namespace Game {

class ISummonMngr :
	public Entity {
public:
	virtual void AddBegin() = 0;
	virtual bool AddEntity(size_t id) = 0;
	virtual void AddEnd() = 0;

public:
	static std::shared_ptr<ISummonMngr> Create();
};

}
