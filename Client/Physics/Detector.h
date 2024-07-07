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
	virtual void setPosition(float x, float y) override;

	virtual void onBeginContact(IFixture* another) override;
	virtual void onEndContact(IFixture* another) override;

	virtual std::map<Game::EntityIdType, MapValue>::iterator listBegin() override;
	virtual std::map<Game::EntityIdType, MapValue>::iterator listEnd() override;

protected:
	void createAsCircle(b2Body* body, uint8_t target, b2Vec2 pos, float radius);
	void createAsRows(b2Body* body, uint8_t target, b2Vec2 pos, Rows* tiles);

	void createAsBlockerCircle(b2Body* body, b2Vec2 pos, float radius);

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
	virtual void setPosition(float x, float y) override;

protected:
	void createAsCircle(b2World* world, uint8_t target, b2Vec2 pos, float radius);
	void createAsRows(b2World* world, uint8_t target, b2Vec2 pos, Rows* rows);
	void createAsTiles(b2World* world, uint8_t target, b2Vec2 pos, size_t length, int* tiles);

	void createAsBlockerCircle(b2World* world, b2Vec2 pos, float radius);

protected:
	b2Body* m_body;
};

}
