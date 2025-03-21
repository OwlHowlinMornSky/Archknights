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
#include "Sensor.h"
#include "../Main/MsgId.h"

namespace Physics {

Sensor::Sensor() {}

Sensor::~Sensor() {
	for (b2ShapeId fix : m_fixtures) {
		b2DestroyShape(fix);
	}
	m_fixtures.clear();
}

void Sensor::setPosition(float x, float y) {
	return;
}

void Sensor::onBeginContact(IFixture* another) {
	auto& i = m_list[another->m_id];
	i.entity = another->m_holder;
	if (i.count == 0) {
		auto holder = m_holder.lock();
		if (holder) {
			holder->receiveMessage(Main::MsgId::SensorEnter, another->m_id, (Game::MsgLparamType)&i.entity);
		}
	}
	i.count++;
	return;
}

void Sensor::onEndContact(IFixture* another) {
	auto it = m_list.find(another->m_id);
	if (it == m_list.end())
		return;
	if (it->second.count <= 1) {
		auto holder = m_holder.lock();
		if (holder) {
			holder->receiveMessage(Main::MsgId::SensorExit, another->m_id, (Game::MsgLparamType)&it->second.entity);
		}
		m_list.erase(it);
		return;
	}
	it->second.count--;
	return;
}

void Sensor::createAsCircle(b2BodyId body, uint8_t target, b2Vec2 pos, float radius) {
	if (!m_fixtures.empty())
		return;

	b2Circle shape = {};
	shape.radius = radius;
	shape.center = b2Vec2_zero;

	b2ShapeDef fixDef = b2DefaultShapeDef();
	fixDef.isSensor = true;
	fixDef.filter.groupIndex = -1;
	fixDef.filter.maskBits = (target << 8); //0b0000000000001000;
	fixDef.filter.categoryBits = 0x0004; //0b0000000000000100;
	fixDef.userData = (void*)this;
	//fixDef.friction = 0.0f;

	b2ShapeId fixture = b2CreateCircleShape(body, &fixDef, &shape);

	m_fixtures.push_back(fixture);
	m_master = true;
	return;
}

void Sensor::createAsRows(b2BodyId body, uint8_t target, b2Vec2 pos, Rows* tiles) {
	if (!m_fixtures.empty())
		return;

	b2ShapeDef fixDef = b2DefaultShapeDef();
	fixDef.isSensor = true;
	fixDef.filter.groupIndex = -1;
	fixDef.filter.maskBits = (target << 8);// 0b0000000000001000;
	fixDef.filter.categoryBits = 0x0004;// 0b0000000000000100;
	fixDef.userData = (void*)this;
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

		b2Polygon shape = b2MakeOffsetBox(hd, hw, { offset + hd - 0.5f, 0.0f }, 0.0f);
		b2ShapeId fixture = b2CreatePolygonShape(body, &fixDef, &shape);
		m_fixtures.push_back(fixture);

		offset += d;
		i = j;
	}
	m_master = true;
}

void Sensor::createAsBlockerCircle(b2BodyId body, b2Vec2 pos, float radius) {
	if (!m_fixtures.empty())
		return;

	b2Circle shape = {};
	shape.radius = radius;

	b2ShapeDef fixDef = b2DefaultShapeDef();
	fixDef.isSensor = true;
	fixDef.filter.groupIndex = -1;
	fixDef.filter.maskBits = 0x0008;
	fixDef.filter.categoryBits = 0x0004;
	fixDef.userData = (void*)this;
	//fixDef.friction = 0.0f;

	b2ShapeId fixture = b2CreateCircleShape(body, &fixDef, &shape);

	m_fixtures.push_back(fixture);
	m_master = true;
	return;
}

SensorIndependent::SensorIndependent() :
	m_body(b2_nullBodyId) {}

SensorIndependent::~SensorIndependent() {
	if (B2_IS_NON_NULL(m_body)) {
		b2DestroyBody(m_body);
		m_body = b2_nullBodyId;
		m_fixtures.clear();
	}
	return;
}

void SensorIndependent::setPosition(float x, float y) {
	auto angle = b2Body_GetRotation(m_body);
	b2Body_SetTransform(m_body, { x, y }, angle);
	return;
}

void SensorIndependent::createAsCircle(b2WorldId world, uint8_t target, b2Vec2 pos, float radius) {
	if (B2_IS_NON_NULL(m_body))
		return;

	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	m_body = b2CreateBody(world, &bodyDef);
	return Sensor::createAsCircle(m_body, target, pos, radius);
}

void SensorIndependent::createAsRows(b2WorldId world, uint8_t target, b2Vec2 pos, Rows* rows) {
	if (B2_IS_NON_NULL(m_body))
		return;

	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	m_body = b2CreateBody(world, &bodyDef);
	return Sensor::createAsRows(m_body, target, pos, rows);
}

void SensorIndependent::createAsTiles(b2WorldId world, uint8_t target, b2Vec2 pos, size_t length, int* tiles) {
	if (B2_IS_NON_NULL(m_body))
		return;

	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	m_body = b2CreateBody(world, &bodyDef);

	b2ShapeDef fixDef = b2DefaultShapeDef();
	fixDef.isSensor = true;
	fixDef.filter.groupIndex = -1;
	fixDef.filter.maskBits = (target << 8); // 0b0000000000001000;
	fixDef.filter.categoryBits = 0x0004;// 0b0000000000000100;
	fixDef.userData = (void*)this;
	//fixDef.friction = 0.0f;

	size_t i = 0, n = length;
	while (i < n) {
		float x = static_cast<float>(tiles[i << 1]);
		float y = static_cast<float>(tiles[(i << 1) | 1]);

		b2Polygon shape = b2MakeOffsetBox(0.5f, 0.5f, { -x, -y }, 0.0f);
		b2ShapeId fixture = b2CreatePolygonShape(m_body, &fixDef, &shape);
		m_fixtures.push_back(fixture);

		i++;
	}
	m_master = true;
	return;
}

void SensorIndependent::createAsBlockerCircle(b2WorldId world, b2Vec2 pos, float radius) {
	if (B2_IS_NON_NULL(m_body))
		return;

	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = pos;
	bodyDef.fixedRotation = true;
	m_body = b2CreateBody(world, &bodyDef);
	return Sensor::createAsBlockerCircle(m_body, pos, radius);
}

}
