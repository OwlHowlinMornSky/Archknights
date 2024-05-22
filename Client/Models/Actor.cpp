#include "Actor.h"

Game::ActorSpine2::ActorSpine2() {}

Game::ActorSpine2::~ActorSpine2() {}

void Game::ActorSpine2::TriggerAnimation(AnimationEvent type, Direction direction) {}

void Game::ActorSpine2::Update(float dt) {
	if (m_isRolling) {
		float Delta = dt * 10.0f;
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
				}
			}
		}
		if (m_target)
			m_target->Update(dt);
		//m_current->setRotation()
		//set m_current rotate by m_currentRLDirection.
	}
	m_current->Update(dt);
	return;
}

void Game::ActorSpine2::Draw(ME::Camera& camera, ME::Shader& shader) {
	return m_current->Draw(camera, shader);
}

ohms::SpineAnimation* Game::ActorSpine2::GetAnimation(bool back) {
	return (ohms::SpineAnimation*)m_holdPTR[back].get();
}

void Game::ActorSpine2::SetDirection(bool RL, bool FB) {
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
	m_targetDirection = Direction::NotCare;
}
