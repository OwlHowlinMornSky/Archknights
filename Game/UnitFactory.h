#pragma once

#include "Entity.h"
#include <stack>
#include <memory>

namespace game {

class Unit;

class UnitFactory :
	public Entity {
	using Parent = game::Entity;
public:
	UnitFactory();
	virtual ~UnitFactory() override;

public:
	std::shared_ptr<Unit> JoinOneUnit();
	void ReturnUnit(Unit* unit);

protected:
	virtual std::shared_ptr<Unit> CreateUnit() const = 0;

protected:
	std::deque<std::shared_ptr<Unit>> m_units;
	std::stack<size_t> m_storedUnits;
	float m_cnt;

// 继承的
public:
	virtual void OnJoined(size_t id, size_t location) override;
	virtual void OnUpdate(float dt) override;
	virtual void OnKicking() override;
};

}
