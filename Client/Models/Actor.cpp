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

#ifdef ARCHKNIGHTS_LIMITED
#include <spine/Event.h>
#include <spine/Animation.h>
#include <spine/EventData.h>
#endif // ARCHKNIGHTS_LIMITED

namespace Game {

Actor::Actor(std::shared_ptr<ME::IModel> _f) :
	m_isRolling(false),
	m_direction(Direction::FR),
	m_targetDirection(Direction::FR),
	m_currentRLDirection(false),
	m_current(nullptr),
	m_holdPTR(_f),

	m_lastEvent(AnimationEvent::Default)

{
	m_current = (CurrentAnimationClass*)m_holdPTR.get();
	m_current->setListener(this);
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
		cnt_OnStart = 0;
		cnt_StartOver = 0;
		modify->setAnimation(0, info->Begin, false);
		break;
	case AnimationEvent::Idle:
		if (m_lastEvent == AnimationEvent::Attack && info->AttackOut != nullptr) {
			modify->setAnimation(0, info->AttackOut, false)->setMixDuration(0.02f);
			modify->addAnimation(0, info->Idle, true, 0.0f);
		}
		else if (m_lastEvent == AnimationEvent::Stun && info->StunOut != nullptr) {
			modify->setAnimation(0, info->StunOut, false)->setMixDuration(0.02f);
			modify->addAnimation(0, info->Idle, true, 0.0f);
		}
		else
			modify->setAnimation(0, info->Idle, true)->setMixDuration(0.02f);
		break;
	case AnimationEvent::Attack:
		cnt_OnAttack = 0;
		cnt_AttackOver = 0;
		if (m_lastEvent != AnimationEvent::Attack && info->AttackIn != nullptr) {
			modify->setAnimation(0, info->AttackIn, false)->setMixDuration(0.02f);
			modify->addAnimation(0, info->AttackLoop, false, 0.0f);
		}
		else
			modify->setAnimation(0, info->AttackLoop, false)->setMixDuration(0.02f);
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
		cnt_DieOver = 0;
		modify->setAnimation(0, info->Die, false)->setMixDuration(0.2f);
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

void Actor::TriggerAnimationEx(int excode, void* data) {}

void Actor::ChangeStatus(AnimationStatus status) {}

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

void Actor::callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event) {
	switch (type) {
	case spine::EventType_Complete:
		if (entry && entry->getAnimation()) {
			spine::Animation* anim = entry->getAnimation();
			if (anim == m_info.Die) {
				cnt_DieOver++;
			}
			else if (anim == m_info.Begin) {
				cnt_StartOver++;
			}
			else if (anim == m_info.AttackLoop) {
				cnt_AttackOver++;
			}
		}
		break;
	case spine::EventType_Event:
	{
		const spine::String& eventName = event->getData().getName();
		if (eventName == "OnAttack") {
			cnt_OnAttack++;
		}
		else if (eventName == "OnStart") {
			cnt_OnStart++;
		}
		break;
	}
	}
}

int Actor::AnimEventCnt_OnStart() {
	auto res = cnt_OnStart;
	cnt_OnStart = 0;
	return res;
}

int Actor::AnimEventCnt_OnAttack() {
	auto res = cnt_OnAttack;
	cnt_OnAttack = 0;
	return res;
}

int Actor::AnimEvent_DieOver() {
	auto res = cnt_DieOver;
	cnt_DieOver = 0;
	return res;
}

int Actor::AnimEvent_StartOver() {
	auto res = cnt_StartOver;
	cnt_StartOver = 0;
	return res;
}

int Actor::AnimEvent_AttackOver() {
	auto res = cnt_AttackOver;
	cnt_AttackOver = 0;
	return res;
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
	m_target(nullptr),

	m_lastEvent(AnimationEvent::Default)

{
	m_holdPTR[0] = _f;
	m_holdPTR[1] = _b;
	m_current = GetAnimation(m_currentFBDirection);
	GetAnimation(false)->setListener(this);
	GetAnimation(true)->setListener(this);
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
#ifdef ARCHKNIGHTS_LIMITED
	bool wantToChangeFace = false;
	bool wantToChangeLR = false;
	bool tryToPlayBackFailed = false;

	if (direction != Direction::NotCare && direction != m_direction) {
		// 需求的前后面不同。
		if (m_currentFBDirection != ((static_cast<char>(direction) & 0x02) != 0))
			wantToChangeFace = true;
		// 需求的左右面不同。
		if (((static_cast<char>(m_direction) ^ static_cast<char>(direction)) & 0x01) != 0)
			wantToChangeLR = true;
	}
	if (m_currentFBDirection != wantToChangeFace) { // 想要播放反面
		CurrentAnimationClass* modify = GetAnimation(true);
		AnimationInfo* info = GetInfo(true);
		bool noBackFace = false;
		switch (type) {
		case AnimationEvent::Begin:
			cnt_OnStart = 0;
			cnt_StartOver = 0;
			if (info->Begin != nullptr)
				modify->setAnimation(0, info->Begin, false)->setMixDuration(0.08f);
			else
				noBackFace = true;
			break;
		case AnimationEvent::Idle:
			if (m_lastEvent == AnimationEvent::Attack && info->AttackOut != nullptr) {
				modify->setAnimation(0, info->AttackOut, false)->setMixDuration(0.08f);
				modify->addAnimation(0, info->Idle, true, 0.0f);
			}
			else if (m_lastEvent == AnimationEvent::Stun && info->StunOut != nullptr) {
				modify->setAnimation(0, info->StunOut, false)->setMixDuration(0.08f);
				modify->addAnimation(0, info->Idle, true, 0.0f);
			}
			else if (info->Idle != nullptr)
				modify->setAnimation(0, info->Idle, true)->setMixDuration(0.08f);
			else
				noBackFace = true;
			break;
		case AnimationEvent::Attack:
			cnt_OnAttack = 0;
			cnt_AttackOver = 0;
			if (m_lastEvent != AnimationEvent::Attack && info->AttackIn != nullptr) {
				modify->setAnimation(0, info->AttackIn, false)->setMixDuration(0.08f);
				modify->addAnimation(0, info->AttackLoop, false, 0.0f);
			}
			else if (info->AttackLoop != nullptr)
				modify->setAnimation(0, info->AttackLoop, false)->setMixDuration(0.08f);
			else
				noBackFace = true;
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
			else if (info->Die != nullptr)
				modify->setAnimation(0, info->Die, false);
			else
				noBackFace = true;
			break;
		case AnimationEvent::Die:
			cnt_DieOver = 0;
			if (info->Die != nullptr)
				modify->setAnimation(0, info->Die, false)->setMixDuration(0.08f);
			else
				noBackFace = true;
			break;
		default:
			modify->setAnimation(0, info->Default, false);
		}
		if (noBackFace) {
			wantToChangeFace = !wantToChangeFace;
		}
	}
	if (m_currentFBDirection == wantToChangeFace) { // 想要播放正面
		CurrentAnimationClass* modify = GetAnimation(false);
		AnimationInfo* info = GetInfo(false);
		switch (type) {
		case AnimationEvent::Begin:
			cnt_OnStart = 0;
			cnt_StartOver = 0;
			modify->setAnimation(0, info->Begin, false);
			break;
		case AnimationEvent::Idle:
			if (m_lastEvent == AnimationEvent::Attack && info->AttackOut != nullptr) {
				modify->setAnimation(0, info->AttackOut, false)->setMixDuration(0.08f);
				modify->addAnimation(0, info->Idle, true, 0.0f);
			}
			else if (m_lastEvent == AnimationEvent::Stun && info->StunOut != nullptr) {
				modify->setAnimation(0, info->StunOut, false)->setMixDuration(0.08f);
				modify->addAnimation(0, info->Idle, true, 0.0f);
			}
			else
				modify->setAnimation(0, info->Idle, true)->setMixDuration(0.08f);
			break;
		case AnimationEvent::Attack:
			cnt_OnAttack = 0;
			cnt_AttackOver = 0;
			if (m_lastEvent != AnimationEvent::Attack && info->AttackIn != nullptr) {
				modify->setAnimation(0, info->AttackIn, false)->setMixDuration(0.08f);
				modify->addAnimation(0, info->AttackLoop, false, 0.0f);
			}
			else
				modify->setAnimation(0, info->AttackLoop, false)->setMixDuration(0.08f);
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
			cnt_DieOver = 0;
			modify->setAnimation(0, info->Die, false)->setMixDuration(0.08f);
			break;
		default:
			modify->setAnimation(0, info->Default, false);
		}
	}
	if (wantToChangeFace || wantToChangeLR) {
		m_isRolling = true;
		char tmp = static_cast<char>(m_direction);;
		if (wantToChangeFace) {
			m_target = GetAnimation(!m_currentFBDirection);
			if (wantToChangeLR)
				tmp ^= 0x03;
			else
				tmp ^= 0x02;
		}
		else {
			tmp ^= 0x01;
		}
		m_targetDirection = Direction(tmp);
	}
	m_lastEvent = type;
#endif // !ARCHKNIGHTS_LIMITED
}

void Actor2::TriggerAnimationEx(int excode, void* data) {}

void Actor2::ChangeStatus(AnimationStatus status) {}

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
					m_direction = m_targetDirection;
					SetDirection(toRL, toFB);
					m_currentRLDirection = -1.0f;
				}
			}
			else { // 朝右转
				m_currentRLDirection += Delta;
				if (m_currentRLDirection >= 1.0f) { // 到位
					m_isRolling = false;
					m_direction = m_targetDirection;
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
							m_direction = m_targetDirection;
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
							m_direction = m_targetDirection;
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

void Actor2::callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event) {
	switch (type) {
	case spine::EventType_Complete:
		if (entry && entry->getAnimation()) {
			spine::Animation* anim = entry->getAnimation();
			if (anim == GetInfo(m_currentFBDirection)->Die) {
				cnt_DieOver++;
			}
			else if (anim == GetInfo(m_currentFBDirection)->Begin) {
				cnt_StartOver++;
			}
			else if (anim == GetInfo(m_currentFBDirection)->AttackLoop) {
				cnt_AttackOver++;
			}
		}
		break;
	case spine::EventType_Event:
	{
		const spine::String& eventName = event->getData().getName();
		if (eventName == "OnAttack") {
			cnt_OnAttack++;
		}
		else if (eventName == "OnStart") {
			cnt_OnStart++;
		}
		break;
	}
	}
}

int Actor2::AnimEventCnt_OnStart() {
	auto res = cnt_OnStart;
	cnt_OnStart = 0;
	return res;
}

int Actor2::AnimEventCnt_OnAttack() {
	auto res = cnt_OnAttack;
	cnt_OnAttack = 0;
	return res;
}

int Actor2::AnimEvent_DieOver() {
	auto res = cnt_DieOver;
	cnt_DieOver = 0;
	return res;
}

int Actor2::AnimEvent_StartOver() {
	auto res = cnt_StartOver;
	cnt_StartOver = 0;
	return res;
}

int Actor2::AnimEvent_AttackOver() {
	auto res = cnt_AttackOver;
	cnt_AttackOver = 0;
	return res;
}

CurrentAnimationClass* Actor2::GetAnimation(bool back) {
	return (CurrentAnimationClass*)m_holdPTR[back].get();
}

AnimationInfo* Actor2::GetInfo(bool back) {
	return m_info + back;
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
