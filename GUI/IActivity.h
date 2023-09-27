#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "ICarnival.h"

namespace GUI {

class IActivity {
public:
	IActivity();
	virtual ~IActivity();

public:
	virtual bool isIndependent() const;
	virtual void runIndependently();

	virtual void handleEvent(const sf::Event& evt);
	virtual void update(float dt);

	virtual void start(ICarnival& carnival) = 0;
	virtual void stop() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual size_t getID() = 0;
}; // class IActivity

} // namespace GUI
