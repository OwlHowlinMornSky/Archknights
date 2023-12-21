#pragma once

#include "Entity.h"
#include "Reference.h"
#include "UnitFactory.h"

namespace game {

class Unit :
	public Entity {
	using Parent = game::Entity;
	friend class UnitFactory;
public:
	Unit();
	virtual ~Unit() override;

public:
	void UnitOnCreated(UnitFactory& factory, size_t productID);

protected:
	virtual void UnitReset() = 0;

protected:
	Reference<UnitFactory> m_factory;
	size_t m_productID;

// 继承的
public:
	virtual void OnKicking() override;
};

}

