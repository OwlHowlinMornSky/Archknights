#pragma once


namespace Game {

class IActor {
public:
	enum class AnimationEvent : char {
		None = 0,
		Begin,
		Idle,
		Attack,
		AttackDown,
		Stun,
		Die,
		Skill0,
		Skill1,
		Skill2,
		Skill3,
		Skill4,
		Skill5,
		Skill6,
		Skill7
	};

	enum class Direction : char {
		FR = 0,
		FL,
		BR,
		BL,
		NotCare
	};

public:
	IActor() = default;
	virtual ~IActor() = default;

	virtual void TriggerAnimation(AnimationEvent type, Direction direction) = 0;
	virtual void TriggerAnimationEx(int excode, void* data) {}
};

}
