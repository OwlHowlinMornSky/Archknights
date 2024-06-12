#pragma once

#include "IDetector.h"
#include "IBody.h"
#include "IWall.h"

#include <memory>

namespace Physics {

enum FilterType : uint8_t {
	ArmyStand  = 0x01,
	ArmyFly    = 0x02,
	ArmyWall   = 0x04,

	RESERVED0  = 0x08,

	EnemyStand = 0x10,
	EnemyFly   = 0x20,

	RESERVED1  = 0x40,
	RESERVED2  = 0x80,
};

class IWorld {
public:
	IWorld() = default;
	virtual ~IWorld() = default;

public:
	virtual void Update(float dt) = 0;

	virtual std::unique_ptr<IWall> CreateWall() = 0;

	virtual std::unique_ptr<IBody> CreateBodyTowerCircle(float x, float y, uint8_t type = FilterType::ArmyStand) = 0;
	virtual std::unique_ptr<IBody> CreateBodyMoverCircle(float x, float y, uint8_t type = FilterType::EnemyStand, bool canBeBlocked = true) = 0;
	virtual std::unique_ptr<IBody> CreateBodyMoverRectangle(float x, float y, uint8_t type = FilterType::EnemyStand, bool canBeBlocked = true) = 0;

	virtual std::unique_ptr<IDetector> CreateDetectorCircle(uint8_t target, float x, float y, float radius) = 0;
	virtual std::unique_ptr<IDetector> CreateDetectorRows(uint8_t target, float x, float y, Rows* rows) = 0;
	virtual std::unique_ptr<IDetector> CreateDetectorTiles(uint8_t target, float x, float y, size_t length, int* tiles) = 0;

public:
	static std::unique_ptr<IWorld> CreateWorld();
};

}
