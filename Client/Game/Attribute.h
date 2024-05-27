#pragma once


namespace Game {

struct Attribute {
	enum ModifyType : int {
		Grow = 0,
		Percent,
		Add,
		Times,

		COUNT
	};

	using ValueType = float;

	bool changed;

	ValueType original;
	ValueType effective;

	ValueType modify[ModifyType::COUNT];

	Attribute() :
		changed(false),
		original(1),
		effective(1),
		modify{ 0, 0, 0, 1 } {}

	ValueType GetValue() noexcept {
		if (changed) {
			effective = ((original + modify[Grow]) * (modify[Percent] + 1) + modify[Add]) * modify[Times];
			changed = false;
		}
		return effective;
	}
};

}
