#pragma once

#include <glm/vec3.hpp>

namespace game {

class Entity {
public:
	Entity() = default;
	virtual ~Entity() = default;

public:
	virtual void enter();
	virtual void update(long long dt);
	virtual void exit();

public:
	glm::vec3 getPosition() const {
		return m_position;
	}
	void setPosition(glm::vec3 pos) {
		m_position = pos;
		Changed_Position();
	}

	glm::vec3 getRotation() const {
		return m_rotation;
	}
	void setRotation(glm::vec3 rot) {
		m_rotation = rot;
		Changed_Rotation();
	}

	glm::vec3 getScale() const {
		return m_scale;
	}
	void setScale(glm::vec3 scl) {
		m_scale = scl;
		Changed_Scale();
	}

protected:
	virtual void Changed_Position();
	virtual void Changed_Rotation();
	virtual void Changed_Scale();

protected:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
};

}
