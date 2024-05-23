#include "char_128_plosis.h"

#include "../Game/MsgResult.h"

Units::Char_128_Plosis::Char_128_Plosis() {}

Units::Char_128_Plosis::~Char_128_Plosis() {}

void Units::Char_128_Plosis::OnJoined() {
	m_actor->TriggerAnimation(Game::IActor::AnimationEvent::Begin);
}

void Units::Char_128_Plosis::OnKicking() {
	m_actor->Exit();
}

void Units::Char_128_Plosis::FixedUpdate(float dt) {}

Game::MsgResultType Units::Char_128_Plosis::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	return Game::MsgResult::OK;
}

void Units::Char_128_Plosis::OnPositionChanged() {
	m_actor->SetPosition(m_position[0], m_position[1], 0.0f);
}
