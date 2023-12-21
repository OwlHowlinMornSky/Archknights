#include "Entity.h"
#include "GameGlobal.h"
#include "GameBoard.h"

#include <iostream>

namespace game {

Entity::Entity() :
	m_updatable(false),

	m_position(),
	m_rotation(),
	m_scale(1.0f),

	m_id(0),
	m_location(0) {}

void Entity::OnJoined(size_t id, size_t location) {
	m_id = id;
	m_location = location;

	// for test
	std::cout << "Join: " << this << ", ID: " << id << ", Location: " << location << std::endl;
}

void Entity::OnUpdate(float dt) {}

void Entity::OnKicking() {
	m_id = 0;
}

void Entity::OnPositionChanged() {}

void Entity::OnRotationChanged() {}

void Entity::OnScaleChanged() {}

void Entity::KickSelf() const {
	Global::instance()->data.board->KickEntity(m_location);
}

}
