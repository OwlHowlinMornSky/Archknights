#pragma once

#include "../GameThings/EntityFactory.h"
#include "../Models/IAnimationSpine.h"

namespace Game {

class Char_151_Mytle_Factory final :
	public EntityFactory {
public:
	virtual bool Load() override;
	virtual bool CreateEntity(std::shared_ptr<Entity>& ptr, void* createInfo) override;

protected:
	bool m_double;
	std::unique_ptr<ohms::ISpinePose> m_pose[2];
};

}
