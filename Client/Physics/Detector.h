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
	void CreateRows(b2Body* body, uint8_t target, b2Vec2 pos, Rows tiles);

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
	void CreateRows(b2World* world, uint8_t target, b2Vec2 pos, Rows rows);
	void CreateTiles(b2World* world, uint8_t target, b2Vec2 pos, size_t length, int* tiles);

protected:
	b2Body* m_body;
};

}
