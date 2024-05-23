#include "char_151_myrtle.h"

#include "../Game/GameGlobal.h"
#include "../Game/GameBoard.h"
#include "../Game/MsgResult.h"
#include <SFML/Window/Event.hpp>

Units::Char_151_Myrtle::Char_151_Myrtle() {
}

Units::Char_151_Myrtle::~Char_151_Myrtle() {}

void Units::Char_151_Myrtle::OnJoined() {
	//m_actor->SetPosition(-5.0f, 1.2f, 0.0f);
	m_actor->TriggerAnimation(Game::IActor::AnimationEvent::Begin);
	Game::GameGlobal::board->SubscribeMsg(1, m_location);
}

void Units::Char_151_Myrtle::OnKicking() {
	Game::GameGlobal::board->UnsubscribeMsg(1, m_location);
	m_actor->Exit();
}

void Units::Char_151_Myrtle::FixedUpdate(float dt) {}

Game::MsgResultType Units::Char_151_Myrtle::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	case 1:
	{
		auto e = (sf::Event*)lparam;
		switch (e->type) {
		case sf::Event::KeyPressed:
			switch (e->key.code) {
			case sf::Keyboard::Numpad1:
				m_actor->TriggerAnimation(
					Game::IActor::AnimationEvent::Idle,
					Game::IActor::Direction::FL
				);
				break;
			case sf::Keyboard::Numpad3:
				m_actor->TriggerAnimation(
					Game::IActor::AnimationEvent::Idle,
					Game::IActor::Direction::FR
				);
				break;
			case sf::Keyboard::Numpad7:
				m_actor->TriggerAnimation(
					Game::IActor::AnimationEvent::Idle,
					Game::IActor::Direction::BL
				);
				break;
			case sf::Keyboard::Numpad9:
				m_actor->TriggerAnimation(
					Game::IActor::AnimationEvent::Idle,
					Game::IActor::Direction::BR
				);
				break;
			case sf::Keyboard::Num0:
				m_actor->TriggerAnimation(
					Game::IActor::AnimationEvent::Die,
					Game::IActor::Direction::FR
				);
				break;
			}
			break;
		}
		break;
	}
	default:
		return Game::MsgResult::Unsubscribe;
	}
	return Game::MsgResult::OK;
}

void Units::Char_151_Myrtle::OnPositionChanged() {
	m_actor->SetPosition(m_position.x, m_position.y, 0.0f);
}

#include "CreateInfoForUnit.h"

bool Game::Char_151_Myrtle_Factory::CreateEntity(std::shared_ptr<Entity>& ptr, void* createInfo) {
	CreateInfoForUnit* info = (CreateInfoForUnit*)createInfo;

	auto unit = std::make_shared<Units::Char_151_Myrtle>();
	unit->m_actor = info->actor;

	ptr = unit;

    return true;
}
