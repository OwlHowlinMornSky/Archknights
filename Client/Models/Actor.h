﻿/*
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
#pragma once

#include "../Game/IActor.h"
#include <MysteryEngine/G3D/IModel.h>

#ifdef ARCHKNIGHTS_LIMITED
#include "AnimationSpine.h"
using CurrentAnimationClass = Model::SpineAnimation;
#else
#include "AnimationFrames.h"
using CurrentAnimationClass = Model::AnimationFrames;
#endif // ARCHKNIGHTS_LIMITED

namespace Model {

struct AnimationInfo {
#ifdef ARCHKNIGHTS_LIMITED
	using InfoType = spine::Animation*;
#endif // ARCHKNIGHTS_LIMITED
	InfoType Default;
	InfoType Begin;
	InfoType Idle;
	InfoType AttackIn;
	InfoType AttackLoop;
	InfoType AttackOut;
	InfoType StunIn;
	InfoType StunLoop;
	InfoType StunOut;
	InfoType Die;
	InfoType MoveIn;
	InfoType MoveLoop;
	InfoType MoveOut;

	AnimationInfo() :
		Default(nullptr),
		Begin(nullptr),
		Idle(nullptr),
		AttackIn(nullptr),
		AttackLoop(nullptr),
		AttackOut(nullptr),
		StunIn(nullptr),
		StunLoop(nullptr),
		StunOut(nullptr),
		Die(nullptr),
		MoveIn(nullptr),
		MoveLoop(nullptr),
		MoveOut(nullptr) {}
};

class Actor :
#ifdef ARCHKNIGHTS_LIMITED
	public spine::AnimationStateListenerObject,
#endif // ARCHKNIGHTS_LIMITED
	public Game::IActor {
public:
	Actor(std::shared_ptr<ME::IModel> _f);
	virtual ~Actor();

	virtual bool Setup() override;
	virtual void Clear() override;

public:
	virtual void Exit() override;

	virtual void InitDirection(Direction direction) override;

	virtual void TriggerAnimation(AnimationEvent type, Direction direction) override;
	virtual void TriggerAnimationEx(int excode, void* data) override;
	virtual void TurnDirection(bool isLeft) override;

	virtual void ChangeStatus(AnimationStatus status) override;

	virtual void SetPosition(float x, float y, float z) override;
	virtual void SetOutline(bool enabled) override;

	virtual void SetHit() override;
	virtual void SetInOut(bool in) override;

	virtual void Update(float dt) override;
	virtual void Draw(ME::Camera* camera, ME::Shader* shader) override;

public:
#ifdef ARCHKNIGHTS_LIMITED
	virtual void callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event) override;
#endif // ARCHKNIGHTS_LIMITED

protected:
	void SetDirection(bool RL);

protected:
	bool m_isRolling; // 是否正在翻转
	bool m_hitFlashing;
	char m_isInOut;
	Direction m_direction; // 上次达成的方位
	Direction m_targetDirection; // 翻转的目标方位（若正在翻转）
	float m_hitFlash;
	float m_currentRLDirection; // 当前实际方位（-1.0: 左, 1.0: 右）
	float m_currentInout;
	CurrentAnimationClass* m_current; // 当前渲染的动画
	std::shared_ptr<ME::IModel> m_holdPTR;

	AnimationEvent m_lastEvent;
	AnimationInfo m_info;
};


class Actor2 :
#ifdef ARCHKNIGHTS_LIMITED
	public spine::AnimationStateListenerObject,
#endif // ARCHKNIGHTS_LIMITED
	public Game::IActor {
public:
	Actor2(std::shared_ptr<ME::IModel> _f, std::shared_ptr<ME::IModel> _b);
	virtual ~Actor2();

	virtual bool Setup() override;
	virtual void Clear() override;

public:
	virtual void Exit() override;

	virtual void InitDirection(Direction direction) override;

	virtual void TriggerAnimation(AnimationEvent type, Direction direction) override;
	virtual void TriggerAnimationEx(int excode, void* data) override;
	virtual void TurnDirection(bool isLeft) override;

	virtual void ChangeStatus(AnimationStatus status) override;

	virtual void SetPosition(float x, float y, float z) override;
	virtual void SetOutline(bool enabled) override;

	virtual void SetHit() override;
	virtual void SetInOut(bool in) override;

	virtual void Update(float dt) override;
	virtual void Draw(ME::Camera* camera, ME::Shader* shader) override;

public:
#ifdef ARCHKNIGHTS_LIMITED
	virtual void callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event) override;
#endif // ARCHKNIGHTS_LIMITED

protected:
	CurrentAnimationClass* GetAnimation(bool back);
	AnimationInfo* GetInfo(bool back);

	void SetDirection(bool RL, bool FB);

protected:
	bool m_isRolling; // 是否正在翻转
	bool m_hitFlashing;
	char m_isInOut;
	Direction m_direction; // 上次达成的方位
	Direction m_targetDirection; // 翻转的目标方位（若正在翻转）
	bool m_currentFBDirection; // 当前实际方位（false: 正面, true: 反面）
	float m_currentRLDirection; // 当前实际方位（-1.0: 左, 1.0: 右）
	float m_hitFlash;
	float m_currentInout;
	CurrentAnimationClass* m_current; // 当前渲染的动画
	CurrentAnimationClass* m_target; // 翻面的目标动画（若翻转还未通过关键点）
	std::shared_ptr<ME::IModel> m_holdPTR[2]; // 0: Front, 1: Back

	AnimationEvent m_lastEvent;
	AnimationInfo m_info[2];
};

}
