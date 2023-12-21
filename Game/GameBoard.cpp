#include "GameBoard.h"

namespace game {

GameBoard::GameBoard() :
	m_idCnt(0) {
	m_rootLoader = std::make_shared<RootLoader>();
	JoinEntity(m_rootLoader);
}

GameBoard::~GameBoard() {
	m_rootLoader.reset();
}

bool GameBoard::isEmpty() {
	return m_emptyLocation.size() == m_entities.size();
}

void GameBoard::Update(size_t dt) {
	for (std::shared_ptr<Entity>& e : m_entities)
		if (e && e->isUpdatable())
			e->OnUpdate(dt);
}

void GameBoard::JoinEntity(std::shared_ptr<Entity> entity) {
	if (m_emptyLocation.empty()) {
		m_entities.push_back(entity);
		entity->OnJoined(++m_idCnt, m_entities.size() - 1);
	}
	else {
		size_t location = m_emptyLocation.top();
		m_emptyLocation.pop();
		m_entities[location] = entity;
		entity->OnJoined(++m_idCnt, location);
	}
}

void GameBoard::KickEntity(size_t location) {
	assert(location < m_entities.size());
	m_entities[location]->OnKicking();
	m_entities[location].reset();
	m_emptyLocation.push(location);
}

std::shared_ptr<Entity> GameBoard::EntityAt(size_t location) {
	assert(location < m_entities.size());
	return m_entities[location];
}


}
