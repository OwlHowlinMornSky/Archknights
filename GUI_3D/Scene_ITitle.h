#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../G3D/base.h"

namespace title {

class Scene_ITitle :
	public sf::Drawable {
public:
	virtual void setup(sf::Vector2u size) = 0;
	virtual void update(float dt) = 0;
	virtual void render() = 0;
};

std::unique_ptr<Scene_ITitle> getScene();

} // namespace title
