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

	/**
	 * @brief 定义了栈变类型，正数为 Pause，负数为 Stop，0 无效。
	*/
	enum Transition : int {
		Switch = 1, // 1 个目标。当前 Activity 会被弹出，目标将被压入栈并运行。
		Push      , // 1 个目标。其将被压入栈中并运行。
		Pop       , // 0 个目标。当前 Activity 会被弹出，新的栈顶将运行，若栈空则退出。
		PopTo     , // 1 个目标。如果目标在栈中，则栈会被弹至目标，目标在栈顶且将运行，否则无效果。
		PopPush   , // 2 个目标。如果目标0在栈中，则栈会被弹至之，目标1被压入且将运行，否则与 Push 相同。
		Exit        // 0 个目标。如果 迁出类型 为 Pause，则尝试退出；若为 Stop，则为强制退出。
	};

public:
	ICarnival();

	virtual ~ICarnival();

public:
	/**
	 * @brief 开始执行。
	*/
	virtual void run() = 0;

	/**
	 * @brief 给非独立 Activity 用的退出运行的方法。
	*/
	virtual void cancelKeepRunning() = 0;

	/**
	 * @brief 给 Activity 用的设置变迁的方法。
	 * @param t 变迁类型。
	*/
	virtual void setTransition(int t, size_t a0 = 0, size_t a1 = 0) = 0;

	/**
	 * @brief 取得该 Carnival 管理的 RenderWindow。
	*/
	virtual sf::RenderWindow& getRenderWindow() = 0;

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
}; // class ICarnival

} // namespace GUI
