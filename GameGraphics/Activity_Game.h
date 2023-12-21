﻿#pragma once

#include "../GUI/Window.h"

namespace game {

class Activity_Game final :
	public GUI::Activity {
public:
	Activity_Game();
	virtual ~Activity_Game() noexcept override;

protected:
	virtual bool start(GUI::Window& wnd) noexcept override;
	virtual void stop() noexcept override;

public:
	virtual bool handleEvent(const sf::Event& evt) override;
	virtual void update(sf::Time dtime) override;

	virtual void OnEnterSysloop() noexcept override;
	virtual void OnExitSysloop() noexcept override;

protected:
	ohms::Ref<GUI::Window> r;
};

}