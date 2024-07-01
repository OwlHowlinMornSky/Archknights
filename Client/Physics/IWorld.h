/*
*    Archknights
*
*    Copyright (C) 2023-2024  Tyler Parret True
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published
*    by the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* @Authors
*    Tyler Parret True <mysteryworldgod@outlook.com><https://github.com/OwlHowlinMornSky>
*/
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
	virtual void update(float dt) = 0;

	virtual std::unique_ptr<IWall> createWall() = 0;

	virtual std::unique_ptr<IBody> createBodyTowerCircle(float x, float y, uint8_t type = FilterType::ArmyStand) = 0;
	virtual std::unique_ptr<IBody> createBodyMoverCircle(float x, float y, uint8_t type = FilterType::EnemyStand) = 0;
	virtual std::unique_ptr<IBody> createBodyMoverRectangle(float x, float y, uint8_t type = FilterType::EnemyStand) = 0;

	virtual std::unique_ptr<IDetector> createDetectorCircle(uint8_t target, float x, float y, float radius) = 0;
	virtual std::unique_ptr<IDetector> createDetectorRows(uint8_t target, float x, float y, Rows* rows) = 0;
	virtual std::unique_ptr<IDetector> createDetectorTiles(uint8_t target, float x, float y, size_t length, int* tiles) = 0;

public:
	static std::unique_ptr<IWorld> CreateWorld();
};

}
