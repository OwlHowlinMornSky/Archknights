#pragma once

#include "Entity.h"
#include <stack>
#include <memory>

namespace game {

class Unit;

class UnitFactory :
	public Entity {
	using Parent = game::Entity;
	friend class Unit;
public:
	UnitFactory();
	virtual ~UnitFactory() override;

public:
	std::shared_ptr<Unit> JoinOneUnit();

protected:
	virtual std::shared_ptr<Unit> CreateUnit() const = 0;

	void ReturnUnit(size_t productID);

protected:
	std::deque<std::shared_ptr<Unit>> m_units;
	std::stack<size_t> m_storedUnits;

// 继承的
public:
	virtual void OnKicking();
};

}
