#pragma once

#include "../GUI/Carnival.h"

namespace GUI {

class CarnivalWin32 : public Carnival {

public:
	CarnivalWin32(HWND hwnd, sf::RenderWindow* r_window);

	virtual ~CarnivalWin32() override;

public:
	virtual void run() override;

	virtual void showMessageBox(std::string_view title, std::string_view text, MBInfo info = MBInfo::None) const override;

	virtual void setCloseButton(bool enabled) const override;

protected:
	void runTheActivity();

	virtual std::unique_ptr<IActivity> createActivity(size_t id) const override;

protected:
	HWND m_hwnd;
};

}

