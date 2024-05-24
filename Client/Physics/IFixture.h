#pragma once

#include "../Game/TypeDef.h"

namespace Physics {

class IFixture {
	friend class Detector;
	friend class DetectorIndependent;
protected:
	IFixture() :
		m_master(false),
		m_id(0),
		m_loc(0) {}
public:
	virtual ~IFixture() = default;

public:
	virtual void OnBeginContact(IFixture* another) = 0;
	virtual void OnEndContact(IFixture* another) = 0;

	bool IsMaster() const {
		return m_master;
	}

	void SetId(Game::EntityIdType id) {
		m_id = id;
	}
	void SetLocation(Game::EntityLocationType loc) {
		m_loc = loc;
	}

protected:
	bool m_master;
	Game::EntityIdType m_id;
	Game::EntityLocationType m_loc;
};

}
