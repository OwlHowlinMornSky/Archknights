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
	auto& i = m_list[another->m_id];
	i.count++;
	i.location = another->m_loc;
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

void Detector::CreateCircle(b2Body* body, uint8_t target, b2Vec2 pos, float radius) {
	if (!m_fixtures.empty())
		return;

	b2CircleShape shape;
	shape.m_radius = radius;

	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	fixDef.isSensor = true;
	fixDef.filter.groupIndex = -1;
	fixDef.filter.maskBits = (target << 8); //0b0000000000001000;
	fixDef.filter.categoryBits = 0x0004; //0b0000000000000100;
	fixDef.userData.pointer = (uintptr_t)this;
	//fixDef.friction = 0.0f;

	b2Fixture* fixture = body->CreateFixture(&fixDef);

	m_fixtures.push_back(fixture);
	m_master = true;
	return;
}

void Detector::CreateRows(b2Body* body, uint8_t target, b2Vec2 pos, Rows* tiles) {
	if (!m_fixtures.empty())
		return;

	b2PolygonShape shape;
	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	fixDef.isSensor = true;
	fixDef.filter.groupIndex = -1;
	fixDef.filter.maskBits = (target << 8);// 0b0000000000001000;
	fixDef.filter.categoryBits = 0x0004;// 0b0000000000000100;
	fixDef.userData.pointer = (uintptr_t)this;
	//fixDef.friction = 0.0f;

	size_t i = 0, n = tiles->length;
	float offset = static_cast<float>(tiles->offset);
	while (i < n) {
		uint32_t w = tiles->widths[i];
		if (w == 0) {
			offset += 1.0f;
			i++;
			continue;
		}

		size_t j = i + 1;
		while (j < n && tiles->widths[j] == w)
			j++;

		float d = static_cast<float>(j - i);
		float hd = d / 2.0f;
		float hw = static_cast<float>(w) - 0.5f;

		shape.SetAsBox(hd, hw, { offset + hd - 0.5f, 0.0f }, 0.0f);
		b2Fixture* fixture = body->CreateFixture(&fixDef);
		m_fixtures.push_back(fixture);

		offset += d;
		i = j;
	}
	m_master = true;
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

void DetectorIndependent::CreateCircle(b2World* world, uint8_t target, b2Vec2 pos, float radius) {
	if (m_body)
		return;

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	m_body = world->CreateBody(&bodyDef);
	return Detector::CreateCircle(m_body, target, pos, radius);
}

void DetectorIndependent::CreateRows(b2World* world, uint8_t target, b2Vec2 pos, Rows* rows) {
	if (m_body)
		return;

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	m_body = world->CreateBody(&bodyDef);
	return Detector::CreateRows(m_body, target, pos, rows);
}

void DetectorIndependent::CreateTiles(b2World* world, uint8_t target, b2Vec2 pos, size_t length, int* tiles) {
	if (m_body)
		return;

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	fixDef.isSensor = true;
	fixDef.filter.groupIndex = -1;
	fixDef.filter.maskBits = (target << 8); // 0b0000000000001000;
	fixDef.filter.categoryBits = 0x0004;// 0b0000000000000100;
	fixDef.userData.pointer = (uintptr_t)this;
	//fixDef.friction = 0.0f;

	size_t i = 0, n = length;
	while (i < n) {
		float x = static_cast<float>(tiles[i << 1]);
		float y = static_cast<float>(tiles[(i << 1) | 1]);

		shape.SetAsBox(0.5f, 0.5f, { -x, -y }, 0.0f);
		b2Fixture* fixture = m_body->CreateFixture(&fixDef);
		m_fixtures.push_back(fixture);

		i++;
	}
	m_master = true;
	return;
}

}
