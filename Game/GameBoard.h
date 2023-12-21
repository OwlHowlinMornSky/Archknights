#pragma once

#include "GameGlobal.h"
#include "Entity.h"
#include <deque>
#include <stack>
#include "RootLoader.h"

namespace game {

class GameBoard final {
public:
	GameBoard();
	~GameBoard();

public:
	bool isEmpty();

	void Update(size_t dt);

	void JoinEntity(std::shared_ptr<Entity> entity);
	void KickEntity(size_t location);

	std::shared_ptr<Entity> EntityAt(size_t location);

protected:
	size_t m_idCnt;
	std::deque<std::shared_ptr<Entity>> m_entities;
	std::stack<size_t> m_emptyLocation;
	std::shared_ptr<RootLoader> m_rootLoader;
};

}
