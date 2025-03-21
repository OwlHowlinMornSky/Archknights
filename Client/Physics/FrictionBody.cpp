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
#include "FrictionBody.h"
#include <box2d/box2d.h>

namespace {

b2BodyId g_instance = b2_nullBodyId;

}

Physics::FrictionBody::FrictionBody() :
	m_body(b2_nullBodyId),
	m_bound(b2_nullShapeId)
{}

Physics::FrictionBody::~FrictionBody() {
	if (B2_IS_NON_NULL(m_body)) {
		b2DestroyBody(m_body);
		m_body = b2_nullBodyId;
		m_bound = b2_nullShapeId;
	}
	g_instance = b2_nullBodyId;
}

b2BodyId Physics::FrictionBody::GetFrictionBodyInstance() {
	return g_instance;
}

void Physics::FrictionBody::setGroundSize(int m, int n) {}

void Physics::FrictionBody::addWallTile(int posx, int posy) {}

void Physics::FrictionBody::Create(b2WorldId world) {
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_staticBody;
	bodyDef.fixedRotation = true;
	bodyDef.enableSleep = false;
	m_body = b2CreateBody(world, &bodyDef);

	g_instance = m_body;
}
