#pragma once

#include "../GUI/ActivityDependent.h"
#include "../Audio/IBgm.h"

namespace Activity {

class Act03_Load :
	public GUI::ActivityDependent {
public:
	Act03_Load() noexcept;
	virtual ~Act03_Load() noexcept override = default;

public:
	virtual void start(GUI::ICarnival& carnival) override;
	virtual void stop() noexcept override;
	virtual void pause() noexcept override;
	virtual void resume() noexcept override;
	virtual uint32_t getID() noexcept override;

public:
	virtual void handleEvent(const sf::Event& evt) override;
	virtual void update(sf::Time deltaTime) override;

protected:
	GUI::ICarnival* ref_carnival;
	std::unique_ptr<Audio::IBgm> m_bgm;
};

}
