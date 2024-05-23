#pragma once

#include "../GameThings/EntityFactory.h"
#include "../Models/IAnimationSpine.h"

namespace Game {

class Char_101_Sora_Factory final :
	public EntityFactory {
public:
	virtual bool Load() override;
	virtual bool CreateEntity(std::shared_ptr<Entity>& ptr) override;

protected:
	bool m_double;
	std::unique_ptr<ohms::ISpinePose> m_pose[2];
};

}
