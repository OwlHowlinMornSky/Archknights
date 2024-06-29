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

#include "IBody.h"
#include "Detector.h"
#include <box2d/box2d.h>
#include <vector>
#include <memory>

namespace Physics {

class Body final :
	public IBody {
	friend class World;
public:
	Body();
	virtual ~Body();

public:
	virtual void SetPosition(float x, float y) override;
	virtual const float* GetPosition() const override;
	virtual void SetVelocity(float x, float y) override;

	virtual size_t AddDetectorCircle(uint8_t target, float x, float y, float radius) override;
	virtual size_t AddDetectorRows(uint8_t target, float x, float y, Rows* rows) override;

	virtual IDetector* GetDetector(size_t id) override;

	virtual void SetMove(float maxv, float maxa) override;
	virtual void SetMoveSpeed(float maxv) override;
	virtual void SetMoveAcceleration(float maxa) override;
	virtual void BeginNormal() override;
	virtual void MoveTo(float x, float y) override;
	virtual void BeginUnbalance(bool dontHitWall = false) override;
	virtual void Push(float ix, float iy) override;
	virtual void Pull(float fx, float fy) override;
	virtual void ClearSpeed() override;
	virtual void GetPositionVelocity(float* out_position, float* out_velocity) override;

	virtual void OnBeginContact(IFixture* another) {}
	virtual void OnEndContact(IFixture* another) {}

protected:
	void CreateCircle(b2World* world, uint8_t type, b2Vec2 pos, float radius);
	void CreateCircleEnemy(b2World* world, uint8_t type, b2Vec2 pos, float radius);

protected:
	bool m_isUnbalance;
	float m_maxV;
	float m_maxA;
	b2Body* m_body;
	b2Fixture* m_fixture;
	b2FrictionJoint* m_frictionJoint;
	std::vector<std::unique_ptr<Detector>> m_detectors;
};

}
