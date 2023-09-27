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

	virtual bool start(ICarnival& carnival);

	virtual void stop();

	virtual void pause();

	virtual void resume();

	virtual std::string_view getTransitionTarget() = 0;

}; // class IActivity

} // namespace GUI
