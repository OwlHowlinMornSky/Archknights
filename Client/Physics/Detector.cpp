#include "Detector.h"

namespace Physics {

Physics::Detector::Detector() {}

Physics::Detector::~Detector() {
	for (b2Fixture* fix : m_fixtures) {
		fix->GetBody()->DestroyFixture(fix);
	}
	m_fixtures.clear();
}

void Detector::SetPosition(float x, float y) {
	return;
}

void Detector::OnBeginContact(IFixture* another) {
	m_list[another->m_id].count++;

	printf_s("%zd vs. %zd\n", m_id, another->m_id);
	return;
}

void Detector::OnEndContact(IFixture* another) {
	auto it = m_list.find(another->m_id);
	if (it == m_list.end())
		return;
	if (it->second.count <= 1) {
		m_list.erase(it);
		return;
	}
	it->second.count--;
	return;
}

std::map<Game::EntityIdType, MapValue>::iterator Detector::ListBegin() {
	return m_list.begin();
}

std::map<Game::EntityIdType, MapValue>::iterator Detector::ListEnd() {
	return m_list.end();
}

void Detector::CreateCircle(b2Body* body, b2Vec2 pos, float radius) {
	if (!m_fixtures.empty())
		return;

	b2CircleShape shape;
	shape.m_radius = radius;

	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	fixDef.isSensor = true;
	fixDef.filter.groupIndex = -1;
	fixDef.filter.maskBits = 0b0000000000001000;
	fixDef.filter.categoryBits = 0b0000000000000100;
	fixDef.userData.pointer = (uintptr_t)this;
	//fixDef.friction = 0.0f;

	b2Fixture* fixture = body->CreateFixture(&fixDef);

	m_fixtures.push_back(fixture);
	m_master = true;
	return;
}

DetectorIndependent::DetectorIndependent() :
	m_body(nullptr) {}

DetectorIndependent::~DetectorIndependent() {
	if (m_body) {
		m_body->GetWorld()->DestroyBody(m_body);
		m_body = nullptr;
		m_fixtures.clear();
	}
	return;
}

void DetectorIndependent::SetPosition(float x, float y) {
	auto angle = m_body->GetAngle();
	m_body->SetTransform({ x, y }, angle);
	return;
}

void DetectorIndependent::CreateCircle(b2World* world, b2Vec2 pos, float radius) {
	if (m_body)
		return;

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	m_body = world->CreateBody(&bodyDef);
	return Detector::CreateCircle(m_body, pos, radius);
}

}
