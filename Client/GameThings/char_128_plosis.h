#pragma once

#include "../Game/Entity.h"
#include "IActor.h"
#include <memory>

namespace Units {

class Char_128_Plosis final :
	public Game::Entity {
public:
	Char_128_Plosis();
	virtual ~Char_128_Plosis();

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
