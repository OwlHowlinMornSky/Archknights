#include "World.h"

#include "IFixture.h"
#include "Detector.h"
#include "Body.h"

namespace Physics {

void MyContactListener::BeginContact(b2Contact* contact) {
	IFixture* fix1 = (IFixture*)contact->GetFixtureA()->GetUserData().pointer;
	IFixture* fix2 = (IFixture*)contact->GetFixtureB()->GetUserData().pointer;
	if (fix1->IsMaster())
		fix1->OnBeginContact(fix2);
	if (fix2->IsMaster())
		fix2->OnBeginContact(fix1);
	return;
}

void MyContactListener::EndContact(b2Contact* contact) {
	IFixture* fix1 = (IFixture*)contact->GetFixtureA()->GetUserData().pointer;
	IFixture* fix2 = (IFixture*)contact->GetFixtureB()->GetUserData().pointer;
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

std::unique_ptr<IBody> World::CreateBodyTowerCircle(float x, float y) {
	auto res = std::make_unique<Body>();
	res->CreateCircle(&m_world, { x, y }, 0.25f);
	return std::move(res);
}

std::unique_ptr<IBody> World::CreateBodyMoverCircle(float x, float y) {
	auto res = std::make_unique<Body>();
	res->CreateCircle(&m_world, { x, y }, 0.1f);
	return std::move(res);
}

std::unique_ptr<IBody> World::CreateBodyMoverRectangle(float x, float y) {
	return std::unique_ptr<IBody>();
}

std::unique_ptr<IDetector> World::CreateDetectorCircle(float x, float y, float radius) {
	auto res = std::make_unique<DetectorIndependent>();
	res->CreateCircle(&m_world, { x, y }, radius);
	return std::move(res);
}

std::unique_ptr<IDetector> World::CreateDetectorRows(float x, float y, Rows* tiles) {
	return std::unique_ptr<IDetector>();
}

std::unique_ptr<IDetector> World::CreateDetectorTiles(float x, float y, size_t length, int* tiles) {
	return std::unique_ptr<IDetector>();
}

}

std::unique_ptr<Physics::IWorld> Physics::IWorld::CreateWorld() {
	return std::make_unique<Physics::World>();
}
