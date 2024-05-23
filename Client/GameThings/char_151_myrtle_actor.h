#pragma once

#include "../Models/Actor.h"

namespace Game {

class Char_151_Mytle_Actor_Vanilla final :
	public ActorSpine2 {
public:
	Char_151_Mytle_Actor_Vanilla(std::shared_ptr<ohms::ISpineAnimation> _f, std::shared_ptr<ohms::ISpineAnimation> _b);
	virtual ~Char_151_Mytle_Actor_Vanilla() = default;

public:
	virtual void TriggerAnimation(AnimationEvent type, Direction direction) override;
};

}
