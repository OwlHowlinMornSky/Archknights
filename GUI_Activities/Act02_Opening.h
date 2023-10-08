#pragma once

#include "..\GUI\ActivityDependent.h"

#include <SFML/Graphics.hpp>

namespace Activity {

class Act02_Opening final :
	public GUI::ActivityDependent {
public:
	Act02_Opening() noexcept;
	virtual ~Act02_Opening() noexcept = default;

public:
	virtual void start(GUI::ICarnival& carnival) override;
	virtual void stop() noexcept override;
	virtual void pause() noexcept override;
	virtual void resume() noexcept override;
	virtual uint32_t getID() noexcept override;

public:
	virtual void handleEvent(const sf::Event& evt) override;
	virtual void update(sf::RenderWindow& window, sf::Time deltaTime) override;

protected:
	int m_status;
	GUI::ICarnival* ref_carnival;
	sf::Time m_timer;
	sf::CircleShape circle[3];
};

}
