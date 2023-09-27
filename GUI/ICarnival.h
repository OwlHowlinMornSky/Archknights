#pragma once

#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

namespace GUI {

/**
 * @brief 接口Carnival: 在 RenderWindow 中运行 Activity 的管理类。
*/
class ICarnival {
public:
	/**
	 * @brief 消息框的额外信息。
	*/
	enum class MBInfo {
		None,
		Info,
		Error
	};

public:
	ICarnival(sf::RenderWindow* r_window);

	virtual ~ICarnival();

public:
	/**
	 * @brief 取得该 Carnival 管理的 RenderWindow。
	*/
	sf::RenderWindow& getRenderWindow();

	/**
	 * @brief 显示一个消息框。
	 * @param title 消息框的标题。
	 * @param text 消息框的内容。
	 * @param info 消息框的额外信息。
	*/
	virtual void showMessageBox(std::string_view title, std::string_view text, MBInfo info = MBInfo::None) const;

	/**
	 * @brief 设置所管理窗口的关闭按钮是否启用。
	*/
	virtual void setCloseButton(bool enabled) const;

protected:
	sf::RenderWindow* ref_window;
}; // class ICarnival

} // namespace GUI
