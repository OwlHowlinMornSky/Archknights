#pragma once

#include "Tower.h"
#include "CommonFactory.h"

namespace game {

class Char001_Myrtle final :
	public Tower {
	using Parent = game::Tower;
public :
	Char001_Myrtle();

public:
	virtual void OnJoined(size_t id, size_t location) override;
	virtual void OnUpdate(float dt) override;
	virtual void OnKicking() override;

protected:
	float m_doki;
	float m_cnt;
	int m_returnCnt;

// 继承的
protected:
	virtual void UnitReset() override;
};

using Char001_Myrtle_Factory = CommonFactory<Char001_Myrtle>;

}
