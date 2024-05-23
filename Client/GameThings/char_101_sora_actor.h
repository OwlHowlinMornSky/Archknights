#pragma once

#include "../Models/Actor.h"

namespace Game {

class Char_101_Sora_Actor_Vanilla final :
	public ActorSpine {
public:
	Char_101_Sora_Actor_Vanilla(std::shared_ptr<ohms::ISpineAnimation> _a);
	virtual ~Char_101_Sora_Actor_Vanilla() = default;

public:
	virtual void TriggerAnimation(AnimationEvent type, Direction direction) override;
};

}
