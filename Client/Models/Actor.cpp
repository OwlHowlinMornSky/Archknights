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

#ifdef ARCHKNIGHTS_LIMITED
#include <spine/Event.h>
#include <spine/Animation.h>
#include <spine/EventData.h>
#endif // ARCHKNIGHTS_LIMITED

namespace {

static constexpr float g_RotationRatio = 9.0f;
static constexpr float g_InOutRatio = 7.0f;
static constexpr float g_HitFadeRatio = 4.5f;
static constexpr float g_HitFadeSpeed = 0.5f;

}

namespace Model {

Actor::Actor(std::shared_ptr<ME::IModel> _f) :
	m_isRolling(false),
	m_direction(Direction::FR),
	m_targetDirection(Direction::FR),
	m_currentRLDirection(1.0f),
	m_current(nullptr),
	m_holdPTR(_f),

	m_lastEvent(AnimationEvent::Default),
	m_hitFlash(0.0f),
	m_hitFlashing(false),

	m_isInOut(0),
	m_currentInout(0.0f)

{
	m_current = (CurrentAnimationClass*)m_holdPTR.get();
	m_current->setListener(this);

	m_shadowRadius = 1.0f;
	m_shadowAlpha = 1.0f;
}

Actor::~Actor() {}

void Actor::setInfoStorage(AnimationInfo* infoStorage) {
	m_infoStorage = infoStorage;
}

bool Actor::setup() {
	return true;
}

void Actor::clear() {}

void Actor::setDirection(Direction direction) {
	if (direction == Direction::BR) direction = Direction::FR;
	if (direction == Direction::BL) direction = Direction::FL;
	m_direction = direction;
	m_targetDirection = direction;
	m_currentRLDirection = (direction == Direction::FL) ? -1.0f : 1.0f;
	m_holdPTR->setRotation(30.0f, (1.0f - m_currentRLDirection) * 90.0f, 0.0f);
}

void Actor::triggerAnimation(AnimationEvent type, Direction direction) {
#ifdef ARCHKNIGHTS_LIMITED
	bool wantToChangeLR = false;
	bool tryToPlayBackFailed = false;

	if (direction != Direction::NotCare && direction != m_direction) {
		// 需求的前后面不同。
		if (direction == Direction::BL)
			direction = Direction::FL;
		else if (direction == Direction::BR)
			direction = Direction::FR;
		// 需求的左右面不同。
		if (((static_cast<char>(m_direction) ^ static_cast<char>(direction)) & 0x01) != 0)
			wantToChangeLR = true;
	}
	// 播放正面
	CurrentAnimationClass* modify = m_current;
	AnimationInfo* info = &m_info;
	switch (type) {
	case AnimationEvent::Begin:
		m_note->OnStart = 0;
		m_note->StartOver = 0;
		modify->setAnimation(0, info->Begin, false);
		break;
	case AnimationEvent::Idle:
		if (m_lastEvent == AnimationEvent::Attack && info->AttackOut != nullptr) {
			modify->setAnimation(0, info->AttackOut, false);
			modify->addAnimation(0, info->Idle, true, 0.0f);
		}
		else if (m_lastEvent == AnimationEvent::Stun && info->StunOut != nullptr) {
			modify->setAnimation(0, info->StunOut, false);
			modify->addAnimation(0, info->Idle, true, 0.0f);
		}
		else
			modify->setAnimation(0, info->Idle, true);
		break;
	case AnimationEvent::Attack:
		m_note->OnAttack = 0;
		m_note->AttackOver = 0;
		if (m_lastEvent != AnimationEvent::Attack && info->AttackIn != nullptr) {
			modify->setAnimation(0, info->AttackIn, false);
			modify->addAnimation(0, info->AttackLoop, false, 0.0f);
		}
		else
			modify->setAnimation(0, info->AttackLoop, false);
		break;
	case AnimationEvent::Stun:
		if (info->StunIn != nullptr && info->StunLoop != nullptr) {
			modify->setAnimation(0, info->StunIn, false);
			modify->addAnimation(0, info->StunLoop, true, 0.0f);
		}
		else if (info->StunLoop != nullptr)
			modify->setAnimation(0, info->StunLoop, true);
		else if (info->StunIn != nullptr)
			modify->setAnimation(0, info->StunIn, false);
		else
			modify->setAnimation(0, info->Die, false);
		break;
	case AnimationEvent::Die:
		m_note->DieOver = 0;
		modify->setAnimation(0, info->Die, false);
		break;
	case AnimationEvent::Move:
		if (info->MoveIn != nullptr && info->MoveLoop != nullptr) {
			modify->setAnimation(0, info->MoveIn, false);
			modify->addAnimation(0, info->MoveLoop, true, 0.0f);
		}
		else if (info->MoveLoop != nullptr)
			modify->setAnimation(0, info->MoveLoop, true);
		else if (info->MoveIn != nullptr)
			modify->setAnimation(0, info->MoveIn, false);
		break;
	default:
		modify->setAnimation(0, info->Default, false);
	}
	if (wantToChangeLR) {
		m_isRolling = true;
		m_targetDirection = direction;
	}
	m_lastEvent = type;
#endif // !ARCHKNIGHTS_LIMITED
}

void Actor::triggerAnimationEx(int excode, void* data) {}

void Actor::turnLeftRight(bool isLeft) {
	if (m_isRolling) {
		switch (m_targetDirection) {
		case Direction::FL:
			if (!isLeft)
				m_targetDirection = Direction::FR;
			break;
		case Direction::FR:
			if (isLeft)
				m_targetDirection = Direction::FL;
			break;
		case Direction::BL:
			if (!isLeft)
				m_targetDirection = Direction::FR;
			break;
		case Direction::BR:
			if (isLeft)
				m_targetDirection = Direction::FL;
			break;
		}
	}
	else if (isLeft != (m_direction == Direction::FL)) {
		m_isRolling = true;
		switch (m_direction) {
		case Direction::FL:
			m_targetDirection = Direction::FR;
			break;
		case Direction::FR:
			m_targetDirection = Direction::FL;
			break;
		case Direction::BL:
			m_targetDirection = Direction::FR;
			break;
		case Direction::BR:
			m_targetDirection = Direction::FL;
			break;
		}
	}
}

void Actor::setStatus(AnimationStatus status) {
	m_info = m_infoStorage[static_cast<size_t>(status)];
}

void Actor::setPosition(float x, float y, float z) {
	IModel::setPosition(x, y, z);
	m_holdPTR->setPosition(x, y, z);
}

void Actor::setHitEffect() {
	m_hitFlash = 1.5f;
	m_hitFlashing = true;
}

void Actor::setInOutEffect(bool in, bool onlyShadow) {
	m_isInOut = in ? 1 : 2;
	m_currentInout = in ? 0.0f : 1.0f;
	m_inOutOnlyShadow = onlyShadow;
	if (!m_inOutOnlyShadow)
		if (m_currentInout < 0.5f) {
			m_current->setColor(0.0f, 0.0f, 0.0f, 2.0f * m_currentInout);
		}
		else {
			float r = 2.0f * (m_currentInout - 0.5f);
			m_current->setColor(r, r, r, 1.0f);
		}
	m_shadowAlpha = m_currentInout;
	return;
}

void Actor::update(float dt) {
	[[unlikely]] if (m_isRolling) {
		float Delta = dt * g_RotationRatio;
		bool toRL = (static_cast<char>(m_targetDirection) & 0x01);
		if (toRL) { // 朝左转
			m_currentRLDirection -= Delta;
			if (m_currentRLDirection <= -1.0f) { // 到位
				m_isRolling = false;
				changeDirection(toRL);
				m_currentRLDirection = -1.0f;
			}
		}
		else { // 朝右转
			m_currentRLDirection += Delta;
			if (m_currentRLDirection >= 1.0f) { // 到位
				m_isRolling = false;
				changeDirection(toRL);
				m_currentRLDirection = 1.0f;
			}
		}
		m_current->setRotation(30.0f, (1.0f - m_currentRLDirection) * 90.0f, 0.0f);
	}
	[[unlikely]] if (m_hitFlashing) {
		m_hitFlash -= (g_HitFadeRatio * m_hitFlash + g_HitFadeSpeed) * dt;
		if (m_hitFlash >= 1.0f)
			m_current->setColor(1.0f, 0.0f, 0.0f, 1.0f);
		else if (m_hitFlash > 0.0f)
			m_current->setColor(1.0f, 1.0f - m_hitFlash, 1.0f - m_hitFlash, 1.0f);
		else {
			m_current->setColor(1.0f, 1.0f, 1.0f, 1.0f);
			m_hitFlashing = false;
		}
	}
	[[unlikely]] if (m_isInOut) {
		float Delta = dt * g_InOutRatio;
		if (m_isInOut == 1) {
			m_currentInout += Delta;
			if (m_currentInout >= 1.0f) {
				m_currentInout = 1.0f;
				m_isInOut = 0;
			}
		}
		else {
			m_currentInout -= Delta;
			if (m_currentInout <= 0.0f) {
				m_currentInout = 0.0f;
				m_isInOut = 0;
				m_waitingForQuit = true;
			}
		}
		if (!m_inOutOnlyShadow)
			if (m_currentInout < 0.5f) {
				m_current->setColor(0.0f, 0.0f, 0.0f, 2.0f * m_currentInout);
			}
			else {
				float r = 2.0f * (m_currentInout - 0.5f);
				m_current->setColor(r, r, r, 1.0f);
			}
		m_shadowAlpha = m_currentInout;
	}
	m_current->update(dt);
	return;
}

void Actor::draw(ME::Camera* camera, ME::Shader* shader) {
	return m_current->draw(camera, shader);
}

void Actor::setOutlineEnabled(bool enabled) {
	return m_holdPTR->setOutlineEnabled(enabled);
}

void Actor::callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event) {
	switch (type) {
	case spine::EventType_Complete:
		if (entry && entry->getAnimation()) {
			spine::Animation* anim = entry->getAnimation();
			if (anim == m_info.Die) {
				m_note->DieOver++;
			}
			else if (anim == m_info.Begin) {
				m_note->StartOver++;
			}
			else if (anim == m_info.AttackLoop) {
				m_note->AttackOver++;
			}
		}
		break;
	case spine::EventType_Event:
	{
		const spine::String& eventName = event->getData().getName();
		if (eventName == "OnAttack") {
			m_note->OnAttack++;
		}
		else if (eventName == "OnStart") {
			m_note->OnStart++;
		}
		break;
	}
	}
}

void Actor::changeDirection(bool RL) {
	if (RL) {
		m_direction = Direction::FL;
	}
	else {
		m_direction = Direction::FR;
	}
	m_targetDirection = m_direction;
}

}
