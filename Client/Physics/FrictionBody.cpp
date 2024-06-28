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

namespace {

b2Body* g_instance = nullptr;

}

Physics::FrictionBody::FrictionBody() :
	m_body(nullptr),
	m_bound(nullptr)
{}

Physics::FrictionBody::~FrictionBody() {
	if (m_body) {
		m_body->GetWorld()->DestroyBody(m_body);
		m_body = nullptr;
		m_bound = nullptr;
	}
	g_instance = nullptr;
}

b2Body* Physics::FrictionBody::GetFrictionBodyInstance() {
	return g_instance;
}

void Physics::FrictionBody::SetSize(int m, int n) {}

void Physics::FrictionBody::AddWallBlock(int posx, int posy) {}

void Physics::FrictionBody::Create(b2World* world) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.fixedRotation = true;
	bodyDef.allowSleep = false;
	m_body = world->CreateBody(&bodyDef);

	g_instance = m_body;
}
