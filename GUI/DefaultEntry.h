#pragma once

#include "IActivity.h"

namespace GUI {

class DefaultEntryDebug : public IActivity {
public:
	DefaultEntryDebug();

	virtual ~DefaultEntryDebug() override;

public:
	virtual void handleEvent(const sf::Event& evt) override;

	virtual void update(float dt) override;

	virtual void start(ICarnival& carnival) override;
	virtual void stop() override;
	virtual void pause() override;
	virtual void resume() override;

	virtual size_t getID() override;

protected:
	ICarnival* ref_carnival;
};

}
