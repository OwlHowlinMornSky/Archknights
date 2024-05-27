#include "Tower.h"

#include "MsgId.h"
#include "../Game/MsgResult.h"

namespace Units {

Tower::Tower() :
	m_active(false),
	m_died(false) {}

Tower::~Tower() {}

void Tower::OnJoined() {
	// 触发动画
	if (m_actor)
		m_actor->TriggerAnimation(Game::IActor::AnimationEvent::Begin);
	// 创建主体
	m_body = Game::GameGlobal::board->m_world->CreateBodyTowerCircle(m_position[0], m_position[1]);
	m_body->SetId(m_id);
	m_body->SetLocation(m_location);
	m_active = false;
	m_died = false;
}

void Tower::OnKicking() {
	m_detector.reset();
	m_body.reset();
	if (m_actor)
		m_actor->Exit();
	m_actor.reset();
}

void Tower::FixedUpdate(float dt) {}

Game::MsgResultType Tower::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	return DefTowerProc(msg, wparam, lparam);
}

void Tower::OnPositionChanged() {
	if (m_actor)
		m_actor->SetPosition(m_position[0], m_position[1], 0.0f);
	if (m_body)
		m_body->SetPosition(m_position[0], m_position[1]);
	if (m_detector)
		m_detector->SetPosition(m_position[0], m_position[1]);
}

Game::MsgResultType Tower::DefTowerProc(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	case Game::MsgId::OnSelecting:
		if (!m_active || m_died)
			return Game::MsgResult::MethodNotAllowed;
		break;
	case Game::MsgId::OnHpDropToZero:
		m_actor->TriggerAnimation(Game::IActor::AnimationEvent::Die);
		m_died = true;
		m_body.reset();
		m_detector.reset();
		break;
	default:
		return DefEntityProc(msg, wparam, lparam);
	}
	return Game::MsgResult::OK;
}

}
