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

#include <box2d/box2d.h>

Physics::Wall::Wall() :
	m_body(b2_nullBodyId),
	m_bound(b2_nullChainId) {}

Physics::Wall::~Wall() {
	if (B2_IS_NON_NULL(m_body)) {
		b2DestroyBody(m_body);
		m_body = b2_nullBodyId;
		m_bound = b2_nullChainId;
	}
}

void Physics::Wall::setGroundSize(int m, int n) {
	if (B2_IS_NON_NULL(m_bound)) {
		b2DestroyChain(m_bound);
		m_bound = b2_nullChainId;
	}

	b2ChainDef chain = b2DefaultChainDef();

	b2Vec2 vertices[4] = {};
	vertices[0] = { 0.0f, 0.0f };
	vertices[1] = { 0.0f, (float)n };
	vertices[2] = { (float)m, (float)n };
	vertices[3] = { (float)m, 0.0f };

	chain.count = 4;
	chain.isLoop = true;
	chain.points = vertices;

	chain.filter.groupIndex = -3;
	chain.filter.maskBits = 0x0001;
	chain.filter.categoryBits = 0x0002;
	//fixDef.userData.pointer = (uintptr_t)this;

	m_bound = b2CreateChain(m_body, &chain);
}

void Physics::Wall::addWallTile(int posx, int posy) {
	//b2PolygonShape shape;

	b2Polygon shape = b2MakeOffsetBox(0.5f, 0.5f, { posx + 0.5f, posy + 0.5f }, 0.0f);

	b2ShapeDef fixDef = b2DefaultShapeDef();
	fixDef.filter.groupIndex = -3;
	fixDef.filter.maskBits = 0x0001;
	fixDef.filter.categoryBits = 0x0002;
	fixDef.friction = 0.0f;
	//fixDef.userData.pointer = (uintptr_t)this;

	m_fixtures.push_back(b2CreatePolygonShape(m_body, &fixDef, &shape));
}

void Physics::Wall::Create(b2WorldId world) {
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_staticBody;
	bodyDef.fixedRotation = true;
	bodyDef.enableSleep = false;
	m_body = b2CreateBody(world, &bodyDef);
	return;
}
