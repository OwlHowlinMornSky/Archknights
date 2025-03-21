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
	m_frictionJoint(b2_nullJointId),
	m_motorJoint(b2_nullJointId),
	m_maxA(0.0f),
	m_maxV(1.0f),
	m_body(b2_nullBodyId),
	m_fixture(b2_nullShapeId) {}

Body::~Body() {
	if (B2_IS_NON_NULL(m_body)) {
		b2DestroyBody(m_body);
		m_body = b2_nullBodyId;
		m_fixture = b2_nullShapeId;
	}
}

void Body::setPosition(float x, float y) {
	b2Body_SetTransform(m_body, { x, y }, b2Body_GetRotation(m_body));
	return;
}

const float* Body::getPosition() {
	b2Vec2 p = b2Body_GetPosition(m_body);
	m_position[0] = p.x;
	m_position[1] = p.y;
	return m_position;
}

void Body::setVelocity(float x, float y) {
	return b2Body_SetLinearVelocity(m_body, { x, y });
}

size_t Body::addDetectorCircle(uint8_t target, float x, float y, float radius) {
	auto res = std::make_unique<Detector>();
	res->createAsCircle(m_body, target, { x, y }, radius);
	m_detectors.push_back(std::move(res));
	return m_detectors.size();
}

size_t Body::addDetectorRows(uint8_t target, float x, float y, Rows* rows) {
	auto res = std::make_unique<Detector>();
	res->createAsRows(m_body, target, { x, y }, rows);
	m_detectors.push_back(std::move(res));
	return m_detectors.size();
}

IDetector* Body::getDetector(size_t id) {
	return m_detectors[id - 1].get();
}

void Body::setMove(float maxv, float maxa) {
	m_maxV = maxv;
	m_maxA = maxa;
	if (!m_isUnbalance) {
		b2Body_SetLinearDamping(m_body, maxa / maxv);
	}
	if (b2MotorJoint_GetMaxForce(m_motorJoint) != 0.0f) {
		b2MotorJoint_SetMaxForce(m_motorJoint, m_maxA);
	}
}

void Body::setMoveTo(bool move, float x, float y) {
	if (move) {
		b2MotorJoint_SetLinearOffset(m_motorJoint, { x, y });
		b2MotorJoint_SetMaxForce(m_motorJoint, m_maxA);
	}
	else {
		b2MotorJoint_SetMaxForce(m_motorJoint, 0.0f);
	}
}

void Body::setMoveSpeed(float maxv) {
	m_maxV = maxv;
	if (!m_isUnbalance) {
		b2Body_SetLinearDamping(m_body, m_maxA / maxv);
	}
}

void Body::setMoveAcceleration(float maxa) {
	m_maxA = maxa;
}

void Body::setStatusNormal() {
	if (m_isUnbalance) {
		b2Filter filter = b2Shape_GetFilter(m_fixture);
		filter.maskBits = 0x0006;
		b2Shape_SetFilter(m_fixture, filter);
		b2MotorJoint_SetMaxForce(m_frictionJoint, 0.0f);
		b2Body_SetLinearDamping(m_body, m_maxA / m_maxV);
	}
	m_isUnbalance = false;
}

void Body::setStatusUnbalance(bool dontHitWall) {
	b2Filter filter = b2Shape_GetFilter(m_fixture);
	filter.maskBits = dontHitWall ? 0x0004 : 0x0006;
	b2Shape_SetFilter(m_fixture, filter);
	if (!m_isUnbalance) {
		b2Body_SetLinearDamping(m_body, 0.0f);
		b2MotorJoint_SetMaxForce(m_frictionJoint, 9.8f);
	}
	m_isUnbalance = true;
}

void Body::impulse(float ix, float iy) {
	b2Body_ApplyLinearImpulseToCenter(m_body, { ix, iy }, true);
}

void Body::pull(float fx, float fy) {
	b2Body_ApplyForceToCenter(m_body, { fx, fy }, true);
}

void Body::clearSpeed() {
	printf_s("Clear!\n");
	b2Body_SetLinearVelocity(m_body, { 0.0f, 0.0f });
}

void Body::getPositionVelocity(float* out_position, float* out_velocity) {
	b2Vec2 v = b2Body_GetLinearVelocity(m_body);
	b2Vec2 p = b2Body_GetPosition(m_body);
	out_position[0] = p.x;
	out_position[1] = p.y;
	out_velocity[0] = v.x;
	out_velocity[1] = v.y;
}

void Body::createAsCircle(b2WorldId world, uint8_t type, b2Vec2 pos, float radius) {
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	bodyDef.automaticMass = false;
	bodyDef.enableSleep = false;
	bodyDef.userData = (void*)this;
	m_body = b2CreateBody(world, &bodyDef);

	b2Circle shape = {};
	shape.radius = radius;
	shape.center = b2Vec2_zero;

	b2ShapeDef fixDef = b2DefaultShapeDef();
	fixDef.filter.groupIndex = -2;
	fixDef.filter.maskBits = 0x0004;
	fixDef.filter.categoryBits = (0x0010 | (type << 8));
	fixDef.userData = (void*)this;

	m_fixture = b2CreateCircleShape(m_body, &fixDef, &shape);

	b2MassData mass = b2Body_GetMassData(m_body);
	mass.mass = 1.0f;
	b2Body_SetMassData(m_body, mass);

	m_master = false;
	return;
}

void Body::createAsCircleMover(b2WorldId world, uint8_t type, b2Vec2 pos, float radius) {
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	bodyDef.automaticMass = false;
	bodyDef.enableSleep = false;
	bodyDef.userData = (void*)this;
	m_body = b2CreateBody(world, &bodyDef);

	b2Circle shape = {};
	shape.radius = radius;
	shape.center = b2Vec2_zero;

	b2ShapeDef fixDef = b2DefaultShapeDef();
	fixDef.filter.groupIndex = -2;
	fixDef.filter.maskBits = 0x0006;
	fixDef.filter.categoryBits = (0x0019 | (type << 8));
	fixDef.friction = 0.0f;
	fixDef.userData = (void*)this;

	m_fixture = b2CreateCircleShape(m_body, &fixDef, &shape);

	b2MassData mass = b2Body_GetMassData(m_body);
	mass.mass = 1.0f;
	b2Body_SetMassData(m_body, mass);

	b2MotorJointDef joint = b2DefaultMotorJointDef();
	joint.bodyIdA = m_body;
	joint.bodyIdB = FrictionBody::GetFrictionBodyInstance();
	joint.maxForce = 0.0f;
	joint.correctionFactor = 0.0f;
	m_frictionJoint = b2CreateMotorJoint(world, &joint);

	b2MotorJointDef motorjoint = b2DefaultMotorJointDef();
	motorjoint.bodyIdA = FrictionBody::GetFrictionBodyInstance();
	motorjoint.bodyIdB = m_body;
	motorjoint.maxForce = 0.0f;
	motorjoint.maxTorque = 0.0f;
	motorjoint.correctionFactor = 1.0f;
	m_motorJoint = b2CreateMotorJoint(world, &motorjoint);

	m_master = false;
	return;
}

void Body::onMoved(float _new_pos_x, float _new_pos_y) {
	m_position[0] = _new_pos_x;
	m_position[1] = _new_pos_y;
}

}
