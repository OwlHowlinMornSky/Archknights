#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Window/Event.hpp>

namespace gamegui {

class SceneCommon final :
	public sf::Drawable {
public:
	SceneCommon();
	virtual ~SceneCommon() override;

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

}
