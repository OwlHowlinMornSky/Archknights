#include "Body.h"

namespace Physics {

Body::Body() :
	m_body(nullptr),
	m_fixture(nullptr) {}

Body::~Body() {
	if (m_body) {
		m_body->GetWorld()->DestroyBody(m_body);
		m_body = nullptr;
		m_fixture = nullptr;
	}
}

void Body::SetPosition(float x, float y) {}

const float* Body::GetPosition() const {
	return &(m_body->GetPosition().x);
}

size_t Body::AddDetectorCircle() {
	return size_t();
}

size_t Body::AddDetectorRows() {
	return size_t();
}

IDetector* Body::GetDetector(size_t id) {
	return nullptr;
}

void Body::CreateCircle(b2World* world, b2Vec2 pos, float radius) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	m_body = world->CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = radius;

	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	fixDef.filter.maskBits = 0b0000000000000110;
	fixDef.filter.categoryBits = 0b0000000000101001;
	fixDef.userData.pointer = (uintptr_t)this;

	m_fixture = m_body->CreateFixture(&fixDef);

	m_master = false;
	return;
}

}
