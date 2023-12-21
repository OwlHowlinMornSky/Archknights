#pragma once

#include "GameGlobal.h"
#include "Entity.h"
#include <deque>
#include <stack>
#include "RootLoader.h"
#include <box2d/box2d.h>

namespace game {

class GameBoard final {
public:
	GameBoard();
	~GameBoard();

	void setup();

public:
	bool isEmpty();
	void setPause(bool pause);

	void Update(float dt);

	void JoinEntity(std::shared_ptr<Entity> entity);
	void KickEntity(size_t location);

	b2World& World() {
		return *m_physics;
	}

	std::shared_ptr<Entity> EntityAt(size_t location);

protected:
	size_t m_idCnt;
	std::deque<std::shared_ptr<Entity>> m_entities;
	std::stack<size_t> m_emptyLocation;
	std::shared_ptr<RootLoader> m_rootLoader;
	std::unique_ptr<b2World> m_physics;
	bool m_paused;
};

}
