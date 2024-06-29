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
#include "Body.h"

#include "FrictionBody.h"

namespace Physics {

Body::Body() :
	m_isUnbalance(false),
	m_frictionJoint(nullptr),
	m_maxA(0.0f),
	m_maxV(1.0f),
	m_body(nullptr),
	m_fixture(nullptr) {}

Body::~Body() {
	if (m_body) {
		m_body->GetWorld()->DestroyBody(m_body);
		m_body = nullptr;
		m_fixture = nullptr;
	}
}

void Body::SetPosition(float x, float y) {
	m_body->SetTransform({ x, y }, m_body->GetAngle());
	return;
}

const float* Body::GetPosition() const {
	return &(m_body->GetPosition().x);
}

void Body::SetVelocity(float x, float y) {
	return m_body->SetLinearVelocity({ x, y });
}

size_t Body::AddDetectorCircle(uint8_t target, float x, float y, float radius) {
	auto res = std::make_unique<Detector>();
	res->CreateCircle(m_body, target, { x, y }, radius);
	m_detectors.push_back(std::move(res));
	return m_detectors.size();
}

size_t Body::AddDetectorRows(uint8_t target, float x, float y, Rows* rows) {
	auto res = std::make_unique<Detector>();
	res->CreateRows(m_body, target, { x, y }, rows);
	m_detectors.push_back(std::move(res));
	return m_detectors.size();
}

IDetector* Body::GetDetector(size_t id) {
	return m_detectors[id - 1].get();
}

void Body::SetMove(float maxv, float maxa) {
	m_maxV = maxv;
	m_maxA = maxa;
	if (!m_isUnbalance) {
		m_body->SetLinearDamping(maxa / maxv);
	}
}

void Body::SetMoveSpeed(float maxv) {
	m_maxV = maxv;
	if (!m_isUnbalance) {
		m_body->SetLinearDamping(m_maxA / maxv);
	}
}

void Body::SetMoveAcceleration(float maxa) {
	m_maxA = maxa;
}

void Body::BeginNormal() {
	if (m_isUnbalance) {
		b2Filter filter = m_fixture->GetFilterData();
		filter.maskBits = 0x0004;
		m_fixture->SetFilterData(filter);
		m_frictionJoint->SetMaxForce(0.0f);
		m_body->SetLinearDamping(m_maxA / m_maxV);
	}
	m_isUnbalance = false;
}

void Body::MoveTo(float x, float y) {
	b2Vec2 f = { x, y };
	f -= m_body->GetPosition();
	f *= m_maxA / f.Length();
	m_body->ApplyForceToCenter(f, true);
	return;
}

void Body::BeginUnbalance(bool dontHitWall) {
	b2Filter filter = m_fixture->GetFilterData();
	filter.maskBits = dontHitWall ? 0x0004 : 0x0006;
	m_fixture->SetFilterData(filter);
	if (!m_isUnbalance) {
		m_body->SetLinearDamping(0.0f);
		m_frictionJoint->SetMaxForce(9.8f);
	}
	m_isUnbalance = true;
}

void Body::Push(float ix, float iy) {
	m_body->ApplyLinearImpulseToCenter({ ix, iy }, true);
}

void Body::Pull(float fx, float fy) {
	m_body->ApplyForceToCenter({ fx, fy }, true);
}

void Body::ClearSpeed() {
	m_body->SetLinearVelocity({ 0.0f, 0.0f });
}

void Body::GetPositionVelocity(float* out_position, float* out_velocity) {
	b2Vec2 v = m_body->GetLinearVelocity();
	b2Vec2 p = m_body->GetPosition();
	out_position[0] = p.x;
	out_position[1] = p.y;
	out_velocity[0] = v.x;
	out_velocity[1] = v.y;
}

void Body::CreateCircle(b2World* world, uint8_t type, b2Vec2 pos, float radius) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	bodyDef.allowSleep = false;
	m_body = world->CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = radius;
	shape.m_p.SetZero();

	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	fixDef.filter.groupIndex = -2;
	fixDef.filter.maskBits = 0x0004;
	fixDef.filter.categoryBits = (0x0010 | (type << 8));
	fixDef.userData.pointer = (uintptr_t)this;

	m_fixture = m_body->CreateFixture(&fixDef);

	b2MassData mass{};
	m_body->GetMassData(&mass);
	mass.mass = 1.0f;
	m_body->SetMassData(&mass);

	m_master = false;
	return;
}

void Body::CreateCircleEnemy(b2World* world, uint8_t type, b2Vec2 pos, float radius) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	bodyDef.allowSleep = false;
	m_body = world->CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = radius;
	shape.m_p.SetZero();

	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	fixDef.filter.groupIndex = -2;
	fixDef.filter.maskBits = 0x0004;
	fixDef.filter.categoryBits = (0x0019 | (type << 8));
	fixDef.friction = 0.0f;
	fixDef.userData.pointer = (uintptr_t)this;

	m_fixture = m_body->CreateFixture(&fixDef);

	b2MassData mass{};
	m_body->GetMassData(&mass);
	mass.mass = 1.0f;
	m_body->SetMassData(&mass);

	b2FrictionJointDef joint;
	joint.maxForce = 0.0f;
	joint.bodyA = m_body;
	joint.bodyB = FrictionBody::GetFrictionBodyInstance();
	m_frictionJoint = (b2FrictionJoint*)m_body->GetWorld()->CreateJoint(&joint);

	m_master = false;
	return;
}

}
