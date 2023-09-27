#pragma once


#include "IActivity.h"

#include <SFML/Graphics.hpp>

class TestActivity : public GUI::IActivity {
public:
	TestActivity(size_t n);

	virtual ~TestActivity() override;

public:
	virtual void handleEvent(const sf::Event& evt) override;

	virtual void update(float dt) override;

	virtual void start(GUI::ICarnival& carnival) override;
	virtual void stop() override;
	virtual void pause() override;
	virtual void resume() override;

	virtual size_t getID() override;

protected:
	GUI::ICarnival* ref_carnival;
	sf::RectangleShape m_shape;
	size_t m_id;
};
