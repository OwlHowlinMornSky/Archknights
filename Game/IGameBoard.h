#pragma once

#include "Entity.h"
#include "UnitFactory.h"
#include <memory>

namespace game {

class IGameBoard {
public:
	IGameBoard() = default;
	virtual ~IGameBoard() = default;

	virtual void setup() = 0;
	virtual void clear() = 0;

public:
	virtual bool isEmpty() = 0;
	virtual void setPause(bool pause) = 0;

	virtual void Update(float dt) = 0;

	/**
	 * @brief 添加一个工厂。
	 * @param factory 工厂。
	 * @return 工厂id（从1开始）。
	*/
	virtual size_t AddFactory(std::shared_ptr<UnitFactory> factory) = 0;
	virtual void JoinEntityFromFactory(size_t id) = 0;

	virtual void JoinEntity(std::shared_ptr<Entity> entity) = 0;
	virtual void KickEntity(size_t location) = 0;

	virtual std::shared_ptr<Entity> EntityAt(size_t location) = 0;
};

}
