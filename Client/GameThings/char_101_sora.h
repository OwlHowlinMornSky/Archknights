#pragma once

#include "EntityFactory.h"

namespace Game {

class Char_101_Sora_Factory final :
	public EntityFactory {
public:
	virtual bool CreateEntity(std::shared_ptr<Entity>& ptr, void* createInfo) override;
};

}

#include "../Game/Entity.h"
#include "IActor.h"

namespace Units {

class Char_101_Sora final :
	public Game::Entity {
public:
	Char_101_Sora();
	virtual ~Char_101_Sora();

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
