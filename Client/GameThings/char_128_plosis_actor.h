#pragma once

#include "../Models/Actor.h"

namespace Game {

class Char_128_Plosis_Actor_Vanilla final :
	public ActorSpine2 {
public:
	Char_128_Plosis_Actor_Vanilla(std::shared_ptr<ohms::ISpineAnimation> _f, std::shared_ptr<ohms::ISpineAnimation> _b);
	virtual ~Char_128_Plosis_Actor_Vanilla() = default;

public:
	virtual void TriggerAnimation(AnimationEvent type, Direction direction) override;
};

class Char_128_Plosis_Actor_Epoque final :
	public ActorSpine2 {
public:
	Char_128_Plosis_Actor_Epoque(std::shared_ptr<ohms::ISpineAnimation> _f, std::shared_ptr<ohms::ISpineAnimation> _b);
	virtual ~Char_128_Plosis_Actor_Epoque() = default;

public:
	virtual void TriggerAnimation(AnimationEvent type, Direction direction) override;
};

}
