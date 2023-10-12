#pragma once

#include "IFontMgr.h"
#include <memory>

namespace GUI {

class FontMgr final :
	public IFontMgr {
public:
	FontMgr() noexcept = default;
	virtual ~FontMgr() = default;

public:
	virtual void load() noexcept override;
	virtual const sf::Font& getFont(GUI::FontType type) const noexcept override;

protected:
	using up_font = std::unique_ptr<sf::Font>;
	sf::Font m_commonContext;
	up_font m_monoNumber;
	up_font m_levelTitle;
};

}
