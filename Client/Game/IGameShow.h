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

#include <MysteryEngine/G3D/IModel.h>
#include <MysteryEngine/G3D/Camera.h>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace Game {

class IGameShow :
	public sf::Drawable {
protected:
	IGameShow();
public:
	virtual ~IGameShow() = default;

public:
	void SetSize(sf::Vector2u size);

	void SetCamera(std::shared_ptr<ME::Camera> cam);
	std::shared_ptr<ME::Camera> GetCamera() const;

	void AddModel(std::shared_ptr<ME::IModel> a);

	void Update(float dt);
	void Draw();

	void TestPoint(sf::Vector2i pt, glm::vec3* outpt);

protected:
	virtual void Render() = 0;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	std::list<std::shared_ptr<ME::IModel>> m_anims;
	std::shared_ptr<ME::Camera> m_camera;
	sf::Sprite m_sp;
	sf::RenderTexture m_rtex;
};

} // namespace Game
