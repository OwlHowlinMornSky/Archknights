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

#include "IWorld.h"
#include "FrictionBody.h"

#include <box2d/id.h>

namespace Physics {

class World final :
	public IWorld {
public:
	World();
	virtual ~World();

public:
	virtual void update() override;

	virtual std::unique_ptr<IWall> createWall() override;

	virtual std::unique_ptr<IBody> createBodyTowerCircle(float x, float y, uint8_t type = FilterType::ArmyStand) override;
	virtual std::unique_ptr<IBody> createBodyMoverCircle(float x, float y, uint8_t type = FilterType::EnemyStand) override;
	virtual std::unique_ptr<IBody> createBodyMoverRectangle(float x, float y, uint8_t type = FilterType::EnemyStand) override;

	virtual std::unique_ptr<IDetector> createDetectorCircle(uint8_t target, float x, float y, float radius) override;
	virtual std::unique_ptr<IDetector> createDetectorRows(uint8_t target, float x, float y, Rows* rows) override;
	virtual std::unique_ptr<IDetector> createDetectorTiles(uint8_t target, float x, float y, size_t length, int* tiles) override;

	virtual std::unique_ptr<IDetector> createBlockerCircle(float x, float y, float radius) override;

protected:
	b2WorldId m_world;
	std::unique_ptr<FrictionBody> m_frictionBody;
};

}
