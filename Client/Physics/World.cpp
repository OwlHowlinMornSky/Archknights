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
#include "World.h"

#include "IFixture.h"
#include "Detector.h"
#include "Body.h"
#include "Wall.h"

#include <box2d/box2d.h>

namespace Physics {

World::World() :
	m_world(b2_nullWorldId) {
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = b2Vec2_zero;
	worldDef.enableSleep = true;
	m_world = b2CreateWorld(&worldDef);

	m_frictionBody = std::make_unique<FrictionBody>();
	m_frictionBody->Create(m_world);
}

World::~World() {
	m_frictionBody.reset();
}

void World::update() {
	b2World_Step(m_world, 1.0f / 30.0f, 4);

	b2SensorEvents sensorEvents = b2World_GetSensorEvents(m_world);
	for (int32_t i = 0, n = sensorEvents.beginCount; i < n; ++i) {
		const auto& evt = sensorEvents.beginEvents[i];
		IFixture* fix1 = (IFixture*)b2Shape_GetUserData(evt.sensorShapeId);
		IFixture* fix2 = (IFixture*)b2Shape_GetUserData(evt.visitorShapeId);
		if (!fix1 || !fix2)
			continue;
		if (fix1->IsMaster())
			fix1->onBeginContact(fix2);
		printf_s("Contact!\n");
	}
	for (int32_t i = 0, n = sensorEvents.endCount; i < n; ++i) {
		const auto& evt = sensorEvents.endEvents[i];
		IFixture* fix1 = (IFixture*)b2Shape_GetUserData(evt.sensorShapeId);
		IFixture* fix2 = (IFixture*)b2Shape_GetUserData(evt.visitorShapeId);
		printf_s("Decontact!\n");
		if (!fix1 || !fix2)
			continue;
		if (fix1->IsMaster())
			fix1->onEndContact(fix2);
	}

	b2BodyEvents bodyEvents = b2World_GetBodyEvents(m_world);
	for (int32_t i = 0, n = bodyEvents.moveCount; i < n; ++i) {
		const auto& evt = bodyEvents.moveEvents[i];
		Body* body = (Body*)evt.userData;
		if (!body)
			continue;
		body->onMoved(evt.transform.p.x, evt.transform.p.y);
	}
}

std::unique_ptr<IWall> World::createWall() {
	auto res = std::make_unique<Wall>();
	res->Create(m_world);
	return std::move(res);
}

std::unique_ptr<IBody> World::createBodyTowerCircle(float x, float y, uint8_t type) {
	auto res = std::make_unique<Body>();
	res->createAsCircle(m_world, type, { x, y }, 0.25f);
	return std::move(res);
}

std::unique_ptr<IBody> World::createBodyMoverCircle(float x, float y, uint8_t type) {
	auto res = std::make_unique<Body>();
	res->createAsCircleMover(m_world, type, { x, y }, 0.1f);
	return std::move(res);
}

std::unique_ptr<IBody> World::createBodyMoverRectangle(float x, float y, uint8_t type) {
	return std::unique_ptr<IBody>();
}

std::unique_ptr<IDetector> World::createDetectorCircle(uint8_t target, float x, float y, float radius) {
	auto res = std::make_unique<DetectorIndependent>();
	res->createAsCircle(m_world, target, { x, y }, radius);
	return std::move(res);
}

std::unique_ptr<IDetector> World::createDetectorRows(uint8_t target, float x, float y, Rows* rows) {
	auto res = std::make_unique<DetectorIndependent>();
	res->createAsRows(m_world, target, { x, y }, rows);
	return std::move(res);
}

std::unique_ptr<IDetector> World::createDetectorTiles(uint8_t target, float x, float y, size_t length, int* tiles) {
	return std::unique_ptr<IDetector>();
}

std::unique_ptr<IDetector> World::createBlockerCircle(float x, float y, float radius) {
	auto res = std::make_unique<DetectorIndependent>();
	res->createAsBlockerCircle(m_world, { x, y }, radius);
	return std::move(res);
}

}

std::unique_ptr<Physics::IWorld> Physics::IWorld::CreateWorld() {
	return std::make_unique<Physics::World>();
}
