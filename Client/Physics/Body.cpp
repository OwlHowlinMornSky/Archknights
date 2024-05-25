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

void Body::SetPosition(float x, float y) {
	m_body->SetTransform({ x, y }, m_body->GetAngle());
	return;
}

const float* Body::GetPosition() const {
	return &(m_body->GetPosition().x);
}

size_t Body::AddDetectorCircle(float x, float y, float radius) {
	auto res = std::make_unique<Detector>();
	res->CreateCircle(m_body, { x, y }, radius);
	m_detectors.push_back(std::move(res));
	return m_detectors.size();
}

size_t Body::AddDetectorRows(float x, float y, Rows rows) {
	auto res = std::make_unique<Detector>();
	res->CreateRows(m_body, { x, y }, rows);
	m_detectors.push_back(std::move(res));
	return m_detectors.size();
}

IDetector* Body::GetDetector(size_t id) {
	return m_detectors[id - 1].get();
}

void Body::CreateCircle(b2World* world, b2Vec2 pos, float radius) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	m_body = world->CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = radius;
	shape.m_p.SetZero();

	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	fixDef.filter.groupIndex = -2;
	fixDef.filter.maskBits = 0b0000000000000110;
	fixDef.filter.categoryBits = 0b0000000000101001;
	fixDef.userData.pointer = (uintptr_t)this;

	m_fixture = m_body->CreateFixture(&fixDef);

	m_master = false;
	return;
}

}
