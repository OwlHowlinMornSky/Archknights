#include "Actor.h"

#include "IActorGroup.h"

namespace Game {

ActorSpine::ActorSpine(std::shared_ptr<ohms::ISpineAnimation> _a) :
	m_isRolling(false),
	m_direction(Direction::FR),
	m_targetDirection(Direction::FR),
	m_currentRLDirection(false),
	m_current(nullptr),
	m_holdPTR(_a) {
	m_current = (ohms::SpineAnimation*)m_holdPTR.get();
}

ActorSpine::~ActorSpine() {}

void ActorSpine::Exit() {
	SetWaitingForQuit();
}

void ActorSpine::InitDirection(Direction direction) {
	if (direction == Direction::BR) direction = Direction::FR;
	if (direction == Direction::BL) direction = Direction::FL;
	m_direction = direction;
	m_targetDirection = direction;
	m_currentRLDirection = (direction == Direction::FL) ? -1.0f : 1.0f;
	m_holdPTR->setRotation(30.0f, (1.0f - m_currentRLDirection) * 90.0f, 0.0f);
}

void ActorSpine::TriggerAnimation(AnimationEvent type, Direction direction) {}

void ActorSpine::SetPosition(float x, float y, float z) {
	m_holdPTR->setPosition(x, y, z);
}

void ActorSpine::SetOutline(bool enabled) {
	return m_holdPTR->SetOutline(enabled);
}

void ActorSpine::Update(float dt) {
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

void ActorSpine::Draw(ME::Camera& camera, ME::Shader& shader) {
	return m_current->Draw(camera, shader);
}

void ActorSpine::SetDirection(bool RL) {
	if (RL) {
		m_direction = Direction::FL;
	}
	else {
		m_direction = Direction::FR;
	}
	m_targetDirection = m_direction;
}


ActorSpine2::ActorSpine2(std::shared_ptr<ohms::ISpineAnimation> _f, std::shared_ptr<ohms::ISpineAnimation> _b) :
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

ActorSpine2::~ActorSpine2() {}

void ActorSpine2::Exit() {
	SetWaitingForQuit();
}

void ActorSpine2::InitDirection(Direction direction) {
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

void ActorSpine2::TriggerAnimation(AnimationEvent type, Direction direction) {
	switch (type) {
	case AnimationEvent::Begin:
		m_current->setAnimation(0, "Start", false);
		m_current->addAnimation(0, "Idle", true, 0.0f);
		break;
	case AnimationEvent::Idle:
		if (m_direction != Direction::NotCare && direction != m_direction) {
			m_targetDirection = direction;
			if (m_currentFBDirection != ((static_cast<char>(m_targetDirection) & 0x02) != 0)) {
				m_target = GetAnimation(!m_currentFBDirection);
				m_target->setAnimation(0, "Idle", true);
			}
			else {
				m_current->setAnimation(0, "Idle", true);
			}
			m_isRolling = true;
		}
		else {
			m_current->setAnimation(0, "Idle", true);
		}
		break;
	case AnimationEvent::Die:
		InitDirection((static_cast<char>(m_targetDirection) & 0x01) ? Direction::FL : Direction::FR);
		m_current->setAnimation(0, "Die", false);
		break;
	}
}

void ActorSpine2::SetPosition(float x, float y, float z) {
	m_holdPTR[0]->setPosition(x, y, z);
	m_holdPTR[1]->setPosition(x, y, z);
}

void ActorSpine2::SetOutline(bool enabled) {
	m_holdPTR[0]->SetOutline(enabled);
	m_holdPTR[1]->SetOutline(enabled);
}

void ActorSpine2::Update(float dt) {
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

void ActorSpine2::Draw(ME::Camera& camera, ME::Shader& shader) {
	return m_current->Draw(camera, shader);
}

ohms::SpineAnimation* ActorSpine2::GetAnimation(bool back) {
	return (ohms::SpineAnimation*)m_holdPTR[back].get();
}

void ActorSpine2::SetDirection(bool RL, bool FB) {
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
