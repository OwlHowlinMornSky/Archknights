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

namespace Physics {

void MyContactListener::BeginContact(b2Contact* contact) {
	IFixture* fix1 = (IFixture*)contact->GetFixtureA()->GetUserData().pointer;
	IFixture* fix2 = (IFixture*)contact->GetFixtureB()->GetUserData().pointer;
	if (!fix1 || !fix2)
		return;
	if (fix1->IsMaster())
		fix1->onBeginContact(fix2);
	if (fix2->IsMaster())
		fix2->onBeginContact(fix1);
	return;
}

void MyContactListener::EndContact(b2Contact* contact) {
	IFixture* fix1 = (IFixture*)contact->GetFixtureA()->GetUserData().pointer;
	IFixture* fix2 = (IFixture*)contact->GetFixtureB()->GetUserData().pointer;
	if (!fix1 || !fix2)
		return;
	if (fix1->IsMaster())
		fix1->onEndContact(fix2);
	if (fix2->IsMaster())
		fix2->onEndContact(fix1);
	return;
}

void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {}

void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {}

World::World() :
	m_world(b2Vec2_zero) {
	m_world.SetAutoClearForces(true);
	m_world.SetAllowSleeping(true);
	//m_world.SetContactFilter();
	m_world.SetContactListener(&m_contactListener);

	m_frictionBody = std::make_unique<FrictionBody>();
	m_frictionBody->Create(&m_world);
}

World::~World() {
	m_frictionBody.reset();
}

void World::update(float dt) {
	m_world.Step(dt, 4, 2);
}

std::unique_ptr<IWall> World::createWall() {
	auto res = std::make_unique<Wall>();
	res->Create(&m_world);
	return std::move(res);
}

std::unique_ptr<IBody> World::createBodyTowerCircle(float x, float y, uint8_t type) {
	auto res = std::make_unique<Body>();
	res->createAsCircle(&m_world, type, { x, y }, 0.25f);
	return std::move(res);
}

std::unique_ptr<IBody> World::createBodyMoverCircle(float x, float y, uint8_t type) {
	auto res = std::make_unique<Body>();
	res->createAsCircleMover(&m_world, type, { x, y }, 0.1f);
	return std::move(res);
}

std::unique_ptr<IBody> World::createBodyMoverRectangle(float x, float y, uint8_t type) {
	return std::unique_ptr<IBody>();
}

std::unique_ptr<IDetector> World::createDetectorCircle(uint8_t target, float x, float y, float radius) {
	auto res = std::make_unique<DetectorIndependent>();
	res->createAsCircle(&m_world, target, { x, y }, radius);
	return std::move(res);
}

std::unique_ptr<IDetector> World::createDetectorRows(uint8_t target, float x, float y, Rows* rows) {
	auto res = std::make_unique<DetectorIndependent>();
	res->createAsRows(&m_world, target, { x, y }, rows);
	return std::move(res);
}

std::unique_ptr<IDetector> World::createDetectorTiles(uint8_t target, float x, float y, size_t length, int* tiles) {
	return std::unique_ptr<IDetector>();
}

}

std::unique_ptr<Physics::IWorld> Physics::IWorld::CreateWorld() {
	return std::make_unique<Physics::World>();
}
