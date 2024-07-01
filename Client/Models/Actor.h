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
#pragma once

#include "../Game/IActor.h"
#include <MysteryEngine/G3D/IModel.h>

#include "AnimationInfo.h"

namespace Model {

class Actor :
#ifdef ARCHKNIGHTS_LIMITED
	public spine::AnimationStateListenerObject,
#endif // ARCHKNIGHTS_LIMITED
	public Game::IActor {
public:
	Actor(std::shared_ptr<ME::IModel> _f);
	virtual ~Actor();

	void setInfoStorage(AnimationInfo* infoStorage);

	virtual bool setup() override;
	virtual void clear() override;

public:
	virtual void setDirection(Direction direction) override;

	virtual void triggerAnimation(AnimationEvent type, Direction direction) override;
	virtual void triggerAnimationEx(int excode, void* data) override;

	virtual void turnLeftRight(bool isLeft) override;

	virtual void setStatus(AnimationStatus status) override;

	virtual void setPosition(float x, float y, float z) override;

	virtual void setHitEffect() override;
	virtual void setInOutEffect(bool in, bool onlyShadow = false) override;

public:
	virtual void update(float dt) override;
	virtual void draw(ME::Camera* camera, ME::Shader* shader) override;

	virtual void setOutlineEnabled(bool enabled) override;

public:
#ifdef ARCHKNIGHTS_LIMITED
	virtual void callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event) override;
#endif // ARCHKNIGHTS_LIMITED

protected:
	void changeDirection(bool RL);

protected:
	bool m_isRolling; // 是否正在翻转
	bool m_hitFlashing;
	bool m_inOutOnlyShadow;
	char m_isInOut;
	Direction m_direction; // 上次达成的方位
	Direction m_targetDirection; // 翻转的目标方位（若正在翻转）
	AnimationEvent m_lastEvent;
	float m_hitFlash;
	float m_currentRLDirection; // 当前实际方位（-1.0: 左, 1.0: 右）
	float m_currentInout;
	CurrentAnimationClass* m_current; // 当前渲染的动画
	std::shared_ptr<ME::IModel> m_holdPTR;

	AnimationInfo m_info;
	AnimationInfo* m_infoStorage;
};

}
