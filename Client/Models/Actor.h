#pragma once

#include "../GameThings/IActor.h"
#include <MysteryEngine/G3D/IModel.h>

#include "AnimationSpine.h"

namespace Game {


class ActorSpine :
	public IActor {

};


class ActorSpine2 :
	public ME::IModel,
	public IActor {
public:
	ActorSpine2();
	virtual ~ActorSpine2();

public:
	virtual void TriggerAnimation(AnimationEvent type, Direction direction) override;

	virtual void Update(float dt) override;
	virtual void Draw(ME::Camera& camera, ME::Shader& shader) override;

protected:
	ohms::SpineAnimation* GetAnimation(bool back);

	void SetDirection(bool RL, bool FB);

protected:
	bool m_isRolling; // 是否正在翻转
	Direction m_direction; // 上次达成的方位
	Direction m_targetDirection; // 翻转的目标方位（若正在翻转）
	bool m_currentFBDirection; // 当前实际方位（false: 正面, true: 反面）
	float m_currentRLDirection; // 当前实际方位（false: 右, true: 左）
	ohms::SpineAnimation* m_current; // 当前渲染的动画
	ohms::SpineAnimation* m_target; // 翻面的目标动画（若翻转还未通过关键点）
	std::shared_ptr<ohms::ISpineAnimation> m_holdPTR[2]; // 0: Front, 1: Back

};


class ActorFrames :
	public IActor {

};


}
