#pragma once

#include <map>
#include "IFixture.h"

namespace Physics {

struct Rows {
	int32_t offset;
	uint32_t length;
	uint32_t* widths;
};

struct MapValue {
	Game::EntityLocationType location;
	size_t count;

	MapValue() :
		location(0),
		count(0) {}
};


class IDetector :
	public IFixture {
protected:
	IDetector() = default;
public:
	virtual ~IDetector() = default;

public:
	virtual void SetPosition(float x, float y) = 0;

	virtual std::map<Game::EntityIdType, MapValue>::iterator ListBegin() = 0;
	virtual std::map<Game::EntityIdType, MapValue>::iterator ListEnd() = 0;
};

}
