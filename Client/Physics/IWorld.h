#pragma once

#include "IDetector.h"
#include "IBody.h"
#include "IWall.h"

#include <memory>

namespace Physics {

class IWorld {
public:
	IWorld() = default;
	virtual ~IWorld() = default;

public:
	virtual void Update(float dt) = 0;

	virtual std::unique_ptr<IWall> CreateWall() = 0;

	virtual std::unique_ptr<IBody> CreateBodyTowerCircle(float x, float y) = 0;
	virtual std::unique_ptr<IBody> CreateBodyMoverCircle(float x, float y) = 0;
	virtual std::unique_ptr<IBody> CreateBodyMoverRectangle(float x, float y) = 0;

	virtual std::unique_ptr<IDetector> CreateDetectorCircle(float x, float y, float radius) = 0;
	virtual std::unique_ptr<IDetector> CreateDetectorRows(float x, float y, Rows rows) = 0;
	virtual std::unique_ptr<IDetector> CreateDetectorTiles(float x, float y, size_t length, int* tiles) = 0;

public:
	static std::unique_ptr<IWorld> CreateWorld();
};

}
