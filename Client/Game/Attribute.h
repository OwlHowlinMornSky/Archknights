#pragma once


namespace Game {

struct Attribute {
	using ValueType = float;

	ValueType original;
	ValueType effective;
	ValueType max, min;

	Attribute() :
		original(0.0f),
		effective(0.0f),
		max(4294967296.0f),
		min(0.0f) {}

	Attribute(ValueType _org) :
		original(_org),
		effective(0.0f),
		max(4294967296.0f),
		min(0.0f) {}

	Attribute(ValueType _org, ValueType _min, ValueType _max) :
		original(_org),
		effective(0.0f),
		max(_max),
		min(_min) {}
};

}
