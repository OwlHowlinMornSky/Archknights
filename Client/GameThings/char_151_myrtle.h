#pragma once

#include "../Game/Entity.h"
#include "../Models/IActor.h"
#include <memory>

namespace Units {

class Char_151_Myrtle final :
	public Game::Entity {
public:
	Char_151_Myrtle();
	virtual ~Char_151_Myrtle();

public:
	virtual void OnJoined();
	virtual void OnKicking();

	virtual void FixedUpdate(float dt);

	virtual Game::MsgResultType ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam);

protected:
	virtual void OnPositionChanged();

public:
	std::shared_ptr<Game::IActor> m_actor;
};

}
