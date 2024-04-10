﻿/*
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

#include <MysteryEngine/G3D/IModel.h>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace Game {

class IGameShow :
	public sf::Drawable {
protected:
	IGameShow() = default;
public:
	virtual ~IGameShow() = default;

public:
	void AddAnimation(std::shared_ptr<ME::IModel> a);

	void Update(float dt);

protected:
	virtual void Render() = 0;
	virtual void UpdateModels(float dt) = 0;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	std::list<std::shared_ptr<ME::IModel>> m_anims;
	sf::Sprite m_sp;
	sf::RenderTexture m_rtex;
};

} // namespace Game
