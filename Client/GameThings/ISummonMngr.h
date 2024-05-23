#pragma once

#include "../Game/Entity.h"
#include <string_view>
#include <memory>

namespace Game {

class ISummonMngr :
	public Entity {
public:
	virtual void AddBegin() = 0;
	virtual void AddEntity(size_t id, std::string_view testname, bool test) = 0;
	virtual void AddEnd() = 0;

public:
	static std::shared_ptr<ISummonMngr> Create();
};

}
