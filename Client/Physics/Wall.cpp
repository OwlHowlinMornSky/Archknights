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
#include "Wall.h"

namespace {

b2Body* g_instance = nullptr;

}

Physics::Wall::Wall() :
	m_body(nullptr),
	m_bound(nullptr) {}

Physics::Wall::~Wall() {
	if (m_body) {
		m_body->GetWorld()->DestroyBody(m_body);
		m_body = nullptr;
		m_bound = nullptr;
	}
	g_instance = nullptr;
}

b2Body* Physics::Wall::GetWallInstance() {
	return g_instance;
}

void Physics::Wall::SetSize(int m, int n) {
	if (m_bound) {
		m_body->DestroyFixture(m_bound);
	}

	b2ChainShape shape;
	b2Vec2 vertices[4];
	vertices[0] = { 0.0f, 0.0f };
	vertices[1] = { 0.0f, (float)n };
	vertices[2] = { (float)m, (float)n };
	vertices[3] = { (float)m, 0.0f };
	shape.CreateLoop(vertices, 4);

	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	fixDef.filter.groupIndex = -3;
	fixDef.filter.maskBits = 0x0001;
	fixDef.filter.categoryBits = 0x0002;
	//fixDef.userData.pointer = (uintptr_t)this;

	m_bound = m_body->CreateFixture(&fixDef);
}

void Physics::Wall::AddWallBlock(int posx, int posy) {
	b2PolygonShape shape;
	shape.SetAsBox(0.5f, 0.5f, { posx + 0.5f, posy + 0.5f }, 0.0f);

	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	fixDef.filter.groupIndex = -3;
	fixDef.filter.maskBits = 0x0001;
	fixDef.filter.categoryBits = 0x0002;
	//fixDef.userData.pointer = (uintptr_t)this;

	m_fixtures.push_back(m_body->CreateFixture(&fixDef));
}

void Physics::Wall::Create(b2World* world) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.fixedRotation = true;
	bodyDef.allowSleep = false;
	m_body = world->CreateBody(&bodyDef);

	g_instance = m_body;

	return;
}
