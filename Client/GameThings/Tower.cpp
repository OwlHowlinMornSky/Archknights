#include "Tower.h"

namespace Units {

Tower::Tower() {}

Tower::~Tower() {}

void Tower::OnJoined() {
	// 触发动画
	m_actor->TriggerAnimation(Game::IActor::AnimationEvent::Begin);
	// 创建主体
	m_body = Game::GameGlobal::board->m_world->CreateBodyTowerCircle(m_position[0], m_position[1]);
	m_body->SetId(m_id);
	m_body->SetLocation(m_location);
}

void Tower::OnKicking() {
	m_detector.reset();
	m_body.reset();
	m_actor->Exit();
}

void Tower::FixedUpdate(float dt) {}

Game::MsgResultType Tower::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	return Game::MsgResultType();
}

void Tower::OnPositionChanged() {
	if (m_actor)
		m_actor->SetPosition(m_position[0], m_position[1], 0.0f);
	if (m_body)
		m_body->SetPosition(m_position[0], m_position[1]);
	if (m_detector)
		m_detector->SetPosition(m_position[0], m_position[1]);
}

}
