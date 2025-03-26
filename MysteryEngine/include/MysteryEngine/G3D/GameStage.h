/*
*    Mystery Engine
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

#include <MysteryEngine/Core/MEAPI.h>
#include <MysteryEngine/G3D/Scene.h>
#include <MysteryEngine/G3D/IModel.h>
#include <MysteryEngine/G3D/ActorGroup.h>

namespace ME {

class ME_API GameStage final :
	public Scene {
public:
	GameStage();
	virtual ~GameStage() override;

public:
	static int init();
	static GameStage& instance();
	static void drop();

	void addGround(std::shared_ptr<ME::IModel> ground);
	void addActor(std::shared_ptr<GameActor> actor);
	void setGroundSize(float x, float y);

public:
	virtual void setup(int code = 0, void* data = nullptr) override;
	virtual void clear() override;

	virtual void update(float dt) override;


protected:
	virtual void onRender() override;
	virtual void onSizeChanged(sf::Vector2u newsize) override;

protected:
	ME::Shader* m_ds;
	sf::RenderTexture m_shadowTex;
	std::shared_ptr<ME::IModel> m_ground;
	ActorGroup m_actors;
};

}
