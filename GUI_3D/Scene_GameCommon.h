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

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Window/Event.hpp>
#include "../Game/GameShow.h"

namespace gamegui {

class Scene_GameCommon final :
	public sf::Drawable,
	public game::GameShow {
public:
	Scene_GameCommon();
	virtual ~Scene_GameCommon() override;

public:
	void update(float dt);

	bool handleEvent(const sf::Event& evt);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		return target.draw(m_sp, states);
	}

protected:
	sf::Sprite m_sp;
	sf::RenderTexture m_rtex;
};

} // namespace gamegui
