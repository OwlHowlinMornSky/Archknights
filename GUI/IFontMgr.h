#pragma once

#include <SFML/Graphics/Font.hpp>

namespace GUI {

enum class FontType : unsigned int {
	CommonContext = 0,
	MonoMunber,
	LevelTitle
};

class IFontMgr {
public:
	IFontMgr() noexcept = default;
	virtual ~IFontMgr() = default;

public:
	virtual void load() noexcept = 0;
	virtual const sf::Font& getFont(FontType type) const noexcept = 0;
};

} // namespace GUI
