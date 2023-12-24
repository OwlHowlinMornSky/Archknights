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
#include "GameBoard.h"

namespace game {

GameBoard::GameBoard() :
	m_paused(false),
	m_idCnt(0) {
	m_physics = std::make_unique<b2World>(b2Vec2_zero);
}

GameBoard::~GameBoard() {
	m_rootLoader.reset();
}

void GameBoard::setup() {
	m_rootLoader = std::make_shared<RootLoader>();
	JoinEntity(m_rootLoader);
}

bool GameBoard::isEmpty() {
	return m_emptyLocation.size() == m_entities.size();
}

void GameBoard::setPause(bool pause) {
	m_paused = pause;
}

void GameBoard::Update(float dt) {
	if (m_paused)
		return;
	m_physics->Step(dt, 3, 5);
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
	assert(entity->getID());
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

} // namespace game
