﻿/*
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

namespace Physics {

void MyContactListener::BeginContact(b2Contact* contact) {
	IFixture* fix1 = (IFixture*)contact->GetFixtureA()->GetUserData().pointer;
	IFixture* fix2 = (IFixture*)contact->GetFixtureB()->GetUserData().pointer;
	if (!fix1 || !fix2)
		return;
	if (fix1->IsMaster())
		fix1->OnBeginContact(fix2);
	if (fix2->IsMaster())
		fix2->OnBeginContact(fix1);
	return;
}

void MyContactListener::EndContact(b2Contact* contact) {
	IFixture* fix1 = (IFixture*)contact->GetFixtureA()->GetUserData().pointer;
	IFixture* fix2 = (IFixture*)contact->GetFixtureB()->GetUserData().pointer;
	if (!fix1 || !fix2)
		return;
	if (fix1->IsMaster())
		fix1->OnEndContact(fix2);
	if (fix2->IsMaster())
		fix2->OnEndContact(fix1);
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
}

World::~World() {}

void World::Update(float dt) {
	m_world.Step(dt, 4, 2);
}

std::unique_ptr<IWall> World::CreateWall() {
	return std::unique_ptr<IWall>();
}

std::unique_ptr<IBody> World::CreateBodyTowerCircle(float x, float y, uint8_t type) {
	auto res = std::make_unique<Body>();
	res->CreateCircle(&m_world, type, false, { x, y }, 0.25f);
	return std::move(res);
}

std::unique_ptr<IBody> World::CreateBodyMoverCircle(float x, float y, uint8_t type, bool canBeBlocked) {
	auto res = std::make_unique<Body>();
	res->CreateCircle(&m_world, type, canBeBlocked, { x, y }, 0.1f);
	return std::move(res);
}

std::unique_ptr<IBody> World::CreateBodyMoverRectangle(float x, float y, uint8_t type, bool canBeBlocked) {
	return std::unique_ptr<IBody>();
}

std::unique_ptr<IDetector> World::CreateDetectorCircle(uint8_t target, float x, float y, float radius) {
	auto res = std::make_unique<DetectorIndependent>();
	res->CreateCircle(&m_world, target, { x, y }, radius);
	return std::move(res);
}

std::unique_ptr<IDetector> World::CreateDetectorRows(uint8_t target, float x, float y, Rows* rows) {
	auto res = std::make_unique<DetectorIndependent>();
	res->CreateRows(&m_world, target, { x, y }, rows);
	return std::move(res);
}

std::unique_ptr<IDetector> World::CreateDetectorTiles(uint8_t target, float x, float y, size_t length, int* tiles) {
	return std::unique_ptr<IDetector>();
}

}

std::unique_ptr<Physics::IWorld> Physics::IWorld::CreateWorld() {
	return std::make_unique<Physics::World>();
}
