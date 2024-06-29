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

#include <memory>
#include <list>
#include "../Game/IActor.h"
#include "../Game/ActorVertex.h"

#include "Shadow.h"

namespace Model {

class ActorGroup final:
	public ME::IModel {
public:
	ActorGroup();
	virtual ~ActorGroup();

public:
	void AddActor(std::shared_ptr<Game::IActor> actor);

public:
	virtual bool Setup();
	virtual void Clear();

	virtual void Update(float dt) override;
	virtual void Draw(ME::Camera* camera, ME::Shader* shader) override;

	void DrawShadow(ME::Camera* camera, ME::Shader* shader);

protected:
	std::list<std::shared_ptr<Game::IActor>> m_actors;
	std::unique_ptr<ME::Shader> m_shader;
	std::unique_ptr<ME::Shader> m_shadowShader;
	Model::Shadow m_shadow;
};

}
