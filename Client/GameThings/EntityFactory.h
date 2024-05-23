#pragma once

#include <memory>
#include "../Game/Entity.h"

namespace Game {

class EntityFactory {
public:
	EntityFactory() = default;
	virtual ~EntityFactory() = default;

public:
	virtual bool Load() = 0;
	virtual bool CreateEntity(std::shared_ptr<Entity>& ptr) = 0;

public:
	static bool Create(std::unique_ptr<EntityFactory>& ptr, size_t entityId);
};

}
