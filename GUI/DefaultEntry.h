#pragma once

#include "IActivity.h"

#include <SFML/Graphics.hpp>

namespace GUI {

class DefaultEntry : public IActivity {
public:
	DefaultEntry();

	virtual ~DefaultEntry() override;

public:
	virtual void handleEvent(const sf::Event& evt);

	virtual void update(float dt);

	virtual void start(ICarnival& carnival);
	virtual void stop();
	virtual void pause();
	virtual void resume();

protected:
	ICarnival* ref_carnival;
	sf::RectangleShape m_shape;
};

}
