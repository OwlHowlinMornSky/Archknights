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
#pragma once

#include "IDetector.h"
#include <box2d/box2d.h>
#include <vector>

namespace Physics {

class Detector :
	public IDetector {
	friend class Body;
public:
	Detector();
	virtual ~Detector();

public:
	virtual void SetPosition(float x, float y) override;

	virtual void OnBeginContact(IFixture* another) override;
	virtual void OnEndContact(IFixture* another) override;

	virtual std::map<Game::EntityIdType, MapValue>::iterator ListBegin() override;
	virtual std::map<Game::EntityIdType, MapValue>::iterator ListEnd() override;

protected:
	void CreateCircle(b2Body* body, uint8_t target, b2Vec2 pos, float radius);
	void CreateRows(b2Body* body, uint8_t target, b2Vec2 pos, Rows* tiles);

protected:
	std::vector<b2Fixture*> m_fixtures;
	std::map<Game::EntityIdType, MapValue> m_list;
};

class DetectorIndependent :
	public Detector {
	friend class World;
public:
	DetectorIndependent();
	virtual ~DetectorIndependent();

public:
	virtual void SetPosition(float x, float y) override;

protected:
	void CreateCircle(b2World* world, uint8_t target, b2Vec2 pos, float radius);
	void CreateRows(b2World* world, uint8_t target, b2Vec2 pos, Rows* rows);
	void CreateTiles(b2World* world, uint8_t target, b2Vec2 pos, size_t length, int* tiles);

protected:
	b2Body* m_body;
};

}
