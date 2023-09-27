#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "ICarnival.h"

namespace GUI {

class IActivity {
public:
	IActivity();
	virtual ~IActivity();

	virtual bool isIndependent() const;

	virtual void runIndependently();

	virtual void handleEvent(const sf::Event& evt);

	virtual void update();

	virtual bool start(ICarnival& carnival);

	virtual void stop();

	virtual void pause();

	virtual void resume();

}; // class IActivity

} // namespace GUI
