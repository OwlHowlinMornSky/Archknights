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
#include <MysteryEngine/G3D/Camera.h>
#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Context.hpp>

namespace ME {

class ME_API Scene :
	public sf::Drawable {
public:
	Scene() = default;
	virtual ~Scene() = default;

public:
	void resize(sf::Vector2u size);

	Camera& getCamera();
	const Camera& getCamera() const;

	virtual void setup(int code = 0, void* data = nullptr);
	virtual void update(float dt);
	void render();

	bool testPoint(sf::Vector2i pt, glm::vec3* outpt);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;

	virtual void onRender();
	virtual void onSizeChanged(sf::Vector2u newsize);

protected:
	ME::Camera m_camera;
	sf::Sprite m_sprite;
	sf::RenderTexture m_renderTexture;
};

}
