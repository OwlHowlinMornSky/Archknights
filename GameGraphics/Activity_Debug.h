#pragma once

#include "../GUI/Window.h"

namespace gamegui {

class Activity_Debug final :
	public GUI::Activity {
public:
	Activity_Debug();
	virtual ~Activity_Debug() noexcept override;

protected:
	virtual bool start(GUI::Window& wnd) noexcept override;
	virtual void stop() noexcept override;

public:
	virtual bool handleEvent(const sf::Event& evt) override;
	virtual void update(sf::Time dtime) override;

	virtual void OnEnterSysloop() noexcept override;
	virtual void OnExitSysloop() noexcept override;

protected:
	GUI::WndRef r;
};

}
