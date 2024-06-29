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

#include "../Game/Stage.h"

#include "../Models/ActorGroup.h"

namespace Scene {

class GameCommon final :
	public Game::Stage {
public:
	GameCommon();
	virtual ~GameCommon() override;

public:
	static int init();
	static void drop();

public:
	virtual void setup(int code = 0, void* data = nullptr) override;
	virtual void clear() override;

	virtual void update(float dt) override;

	virtual void AddGround(std::shared_ptr<ME::IModel> ground) override;
	virtual void AddActor(std::shared_ptr<Game::IActor> actor) override;

	virtual void SetGroundSize(float x, float y) override;

protected:
	virtual void onRender() override;
	virtual void onSizeChanged(sf::Vector2u newsize) override;

protected:
	ME::Shader* m_ds;
	sf::RenderTexture m_shadowTex;
	std::shared_ptr<ME::IModel> m_ground;
	Model::ActorGroup m_actors;
};

} // namespace Scene
