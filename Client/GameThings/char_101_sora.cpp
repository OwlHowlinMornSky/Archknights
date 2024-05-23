#include "char_101_sora.h"

Units::Char_101_Sora::Char_101_Sora() {}

Units::Char_101_Sora::~Char_101_Sora() {}

void Units::Char_101_Sora::OnJoined() {
	m_actor->TriggerAnimation(Game::IActor::AnimationEvent::Begin);
}

void Units::Char_101_Sora::OnKicking() {
	m_actor->Exit();
}

void Units::Char_101_Sora::FixedUpdate(float dt) {}

Game::MsgResultType Units::Char_101_Sora::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	return Game::MsgResultType();
}

void Units::Char_101_Sora::OnPositionChanged() {
	m_actor->SetPosition(m_position.x, m_position.y, 0.0f);
}

#include "CreateInfoForUnit.h"

bool Game::Char_101_Sora_Factory::CreateEntity(std::shared_ptr<Entity>& ptr, void* createInfo) {
	CreateInfoForUnit* info = (CreateInfoForUnit*)createInfo;

	auto unit = std::make_shared<Units::Char_101_Sora>();
	unit->m_actor = info->actor;

	ptr = unit;

	return true;
}
