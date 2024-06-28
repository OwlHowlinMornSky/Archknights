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
#include "IFixture.h"

namespace Physics {

class IBody :
	public IFixture {
protected:
	IBody() = default;
public:
	virtual ~IBody() = default;

public:
	virtual void SetPosition(float x, float y) = 0;
	virtual const float* GetPosition() const = 0;

	virtual size_t AddDetectorCircle(uint8_t target, float x, float y, float radius) = 0;
	virtual size_t AddDetectorRows(uint8_t target, float x, float y, Rows* rows) = 0;

	virtual IDetector* GetDetector(size_t id) = 0;

	virtual void SetMove(float maxv, float maxa) = 0;
	virtual void SetMoveSpeed(float maxv) = 0;
	virtual void SetMoveAcceleration(float maxa) = 0;
	virtual void BeginNormal() = 0;
	virtual void MoveTo(float x, float y) = 0;
	virtual void BeginUnbalance() = 0;
	virtual void Push(float ix, float iy) = 0;
	virtual void Pull(float fx, float fy) = 0;
	virtual void ClearSpeed() = 0;
	virtual void GetPositionVelocity(float* out_position, float* out_velocity) = 0;
};

}
