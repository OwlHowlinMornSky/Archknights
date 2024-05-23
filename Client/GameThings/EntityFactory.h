#pragma once

#include <memory>
#include "../Game/Entity.h"

namespace Game {

class EntityFactory {
public:
	EntityFactory() = default;
	virtual ~EntityFactory() = default;

public:
	virtual bool CreateEntity(std::shared_ptr<Entity>& ptr, void* createInfo) = 0;

public:
	static std::unique_ptr<EntityFactory> Create(size_t entityId);
};

}
