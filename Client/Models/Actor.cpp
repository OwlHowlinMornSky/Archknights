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
#include "Actor.h"

#include "IActorGroup.h"

namespace Game {

Actor::Actor(std::shared_ptr<ME::IModel> _f) :
	m_isRolling(false),
	m_direction(Direction::FR),
	m_targetDirection(Direction::FR),
	m_currentRLDirection(false),
	m_current(nullptr),
	m_holdPTR(_f) {
	m_current = (CurrentAnimationClass*)m_holdPTR.get();
}

Actor::~Actor() {}

void Actor::Exit() {
	SetWaitingForQuit();
}

void Actor::InitDirection(Direction direction) {
	if (direction == Direction::BR) direction = Direction::FR;
	if (direction == Direction::BL) direction = Direction::FL;
	m_direction = direction;
	m_targetDirection = direction;
	m_currentRLDirection = (direction == Direction::FL) ? -1.0f : 1.0f;
	m_holdPTR->setRotation(30.0f, (1.0f - m_currentRLDirection) * 90.0f, 0.0f);
}

void Actor::TriggerAnimation(AnimationEvent type, Direction direction) {
#ifndef ARCHKNIGHTS_LIMITED

#endif // !ARCHKNIGHTS_LIMITED
}

void Actor::SetPosition(float x, float y, float z) {
	this->setPosition(x, y, z);
	m_holdPTR->setPosition(x, y, z);
}

void Actor::SetOutline(bool enabled) {
	return m_holdPTR->SetOutline(enabled);
}

void Actor::Update(float dt) {
	if (m_isRolling) {
		float Delta = dt * 30.0f;
		bool toRL = (static_cast<char>(m_targetDirection) & 0x01);
		if (toRL) { // 朝左转
			m_currentRLDirection -= Delta;
			if (m_currentRLDirection <= -1.0f) { // 到位
				m_isRolling = false;
				SetDirection(toRL);
				m_currentRLDirection = -1.0f;
			}
		}
		else { // 朝右转
			m_currentRLDirection += Delta;
			if (m_currentRLDirection >= 1.0f) { // 到位
				m_isRolling = false;
				SetDirection(toRL);
				m_currentRLDirection = 1.0f;
			}
		}
		m_current->setRotation(30.0f, (1.0f - m_currentRLDirection) * 90.0f, 0.0f);
	}
	m_current->Update(dt);
	return;
}

void Actor::Draw(ME::Camera& camera, ME::Shader& shader) {
	return m_current->Draw(camera, shader);
}

void Actor::SetDirection(bool RL) {
	if (RL) {
		m_direction = Direction::FL;
	}
	else {
		m_direction = Direction::FR;
	}
	m_targetDirection = m_direction;
}


Actor2::Actor2(std::shared_ptr<ME::IModel> _f, std::shared_ptr<ME::IModel> _b) :
	m_isRolling(false),
	m_direction(Direction::FR),
	m_targetDirection(Direction::FR),
	m_currentFBDirection(false),
	m_currentRLDirection(1.0f),
	m_current(nullptr),
	m_target(nullptr) {
	m_holdPTR[0] = _f;
	m_holdPTR[1] = _b;
	m_current = GetAnimation(m_currentFBDirection);
}

Actor2::~Actor2() {}

void Actor2::Exit() {
	SetWaitingForQuit();
}

void Actor2::InitDirection(Direction direction) {
	m_direction = direction;
	m_targetDirection = direction;
	m_currentFBDirection = (static_cast<char>(direction) & 0x02);
	m_currentRLDirection = (static_cast<char>(direction) & 0x01) ? -1.0f : 1.0f;
	m_current = GetAnimation(m_currentFBDirection);
	m_target = nullptr;
	m_holdPTR[0]->setRotation(30.0f, (1.0f - m_currentRLDirection) * 90.0f, 0.0f);
	m_holdPTR[1]->setRotation(30.0f, (1.0f - m_currentRLDirection) * 90.0f, 0.0f);
	return;
}

void Game::Actor2::TriggerAnimation(AnimationEvent type, Direction direction) {
#ifndef ARCHKNIGHTS_LIMITED

#endif // !ARCHKNIGHTS_LIMITED
}

void Actor2::SetPosition(float x, float y, float z) {
	this->setPosition(x, y, z);
	m_holdPTR[0]->setPosition(x, y, z);
	m_holdPTR[1]->setPosition(x, y, z);
}

void Actor2::SetOutline(bool enabled) {
	m_holdPTR[0]->SetOutline(enabled);
	m_holdPTR[1]->SetOutline(enabled);
}

void Actor2::Update(float dt) {
	if (m_isRolling) {
		float Delta = dt * 30.0f;
		bool nowRL = (m_currentRLDirection < 0.0f);
		bool nowFB = m_currentFBDirection;
		bool toRL = (static_cast<char>(m_targetDirection) & 0x01);
		bool toFB = (static_cast<char>(m_targetDirection) & 0x02);
		bool isIn = ((nowRL == toRL) && (nowFB == toFB));
		if (isIn) { // 已经通过关键点，朝目标靠近
			if (toRL) { // 朝左转
				m_currentRLDirection -= Delta;
				if (m_currentRLDirection <= -1.0f) { // 到位
					m_isRolling = false;
					SetDirection(toRL, toFB);
					m_currentRLDirection = -1.0f;
				}
			}
			else { // 朝右转
				m_currentRLDirection += Delta;
				if (m_currentRLDirection >= 1.0f) { // 到位
					m_isRolling = false;
					SetDirection(toRL, toFB);
					m_currentRLDirection = 1.0f;
				}
			}
		}
		else { // 还未通过关键点，向关键点靠近
			if (nowRL) { // 现在在左侧
				m_currentRLDirection += Delta; // 加RL以向右转
				if (m_currentRLDirection >= 0.0f) { // 已经通过关键点
					if (nowFB != toFB) { // 若要翻面
						if (m_target)
							m_current = m_target; // 翻面
						m_target = nullptr;
					}
					m_currentFBDirection = toFB; // 通过关键点后正反面已达标
					if (nowRL == toRL) { // 要往回转
						m_currentRLDirection = -m_currentRLDirection;
						if (m_currentRLDirection <= -1.0f) { // 到位
							m_isRolling = false;
							SetDirection(toRL, toFB);
							m_currentRLDirection = -1.0f;
						}
					}
				}
			}
			else { // 现在在右侧
				m_currentRLDirection -= Delta; // 减RL以向左转
				if (m_currentRLDirection < 0.0f) { // 已经通过关键点
					if (nowFB != toFB) { // 若要翻面
						if (m_target)
							m_current = m_target; // 翻面
						m_target = nullptr;
					}
					m_currentFBDirection = toFB; // 通过关键点后正反面已达标
					if (nowRL == toRL) { // 要往回转
						m_currentRLDirection = -m_currentRLDirection;
						if (m_currentRLDirection >= 1.0f) { // 到位
							m_isRolling = false;
							SetDirection(toRL, toFB);
							m_currentRLDirection = 1.0f;
						}
					}
				}
			}
		}
		if (m_target)
			m_target->Update(dt);
		//set m_current rotate by m_currentRLDirection.
		m_current->setRotation(30.0f, (1.0f - m_currentRLDirection) * 90.0f, 0.0f);
	}
	m_current->Update(dt);
	return;
}

void Actor2::Draw(ME::Camera& camera, ME::Shader& shader) {
	return m_current->Draw(camera, shader);
}

CurrentAnimationClass* Actor2::GetAnimation(bool back) {
	return (CurrentAnimationClass*)m_holdPTR[back].get();
}

void Actor2::SetDirection(bool RL, bool FB) {
	if (FB) {
		if (RL) {
			m_direction = Direction::BL;
		}
		else {
			m_direction = Direction::BR;
		}
	}
	else {
		if (RL) {
			m_direction = Direction::FL;
		}
		else {
			m_direction = Direction::FR;
		}
	}
	m_targetDirection = m_direction;
}

}
