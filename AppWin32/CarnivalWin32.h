#pragma once

#include "../GUI/IActivity.h"

namespace GUI {

class CarnivalWin32 : public ICarnival {

public:
	CarnivalWin32(HWND hwnd, sf::RenderWindow* r_window);

	virtual ~CarnivalWin32() override;

public:
	virtual void run() override;

	virtual void showMessageBox(std::string_view title, std::string_view text, MBInfo info = MBInfo::None) const override;

	virtual void setCloseButton(bool enabled) const override;

protected:
	void runTheActivity();

protected:
	HWND m_hwnd;
};

}

