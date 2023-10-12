#include "FontMgr.h"

namespace GUI {

void FontMgr::load() noexcept {
	m_commonContext.loadFromFile("assets/fonts/SourceHanSansSC-Regular.otf");

	// TO-DO

	return;
}

const sf::Font& FontMgr::getFont(GUI::FontType type) const noexcept {
	switch (type) {
	case FontType::CommonContext:
		break;
	case FontType::MonoMunber:
		if (m_monoNumber != nullptr)
			return *m_monoNumber;
		break;
	case FontType::LevelTitle:
		if (m_levelTitle != nullptr)
			return *m_levelTitle;
		break;
	default:
		break;
	}
	return m_commonContext;
}

} // namespace GUI
