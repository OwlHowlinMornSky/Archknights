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
		None = 0,
		Info,
		Error
	};

	enum class Transition {
		None = 0,
		PauseMe_and_Open,
		StopMe_and_Open,
		Return,
		ReturnTo,
		ReturnTo_and_Open,
		Exit,
		ForceExit
	};

public:
	ICarnival(sf::RenderWindow* r_window);

	virtual ~ICarnival();

public:
	/**
	 * @brief 开始执行。
	*/
	virtual void run() = 0;

	/**
	 * @brief 给非独立 Activity 用的退出运行的方法。
	*/
	void cancelKeepRunning();

	/**
	 * @brief 给 Activity 用的设置变迁的方法。
	 * @param t 变迁类型。
	*/
	void setTransition(Transition t);

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
	void handleTransition();

protected:
	bool m_keepRunning;
	sf::RenderWindow* ref_window;
	std::unique_ptr<IActivity> m_running;
	Transition m_transition;
}; // class ICarnival

} // namespace GUI
