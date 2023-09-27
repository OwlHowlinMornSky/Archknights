#pragma once

#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

namespace GUI {

/**
 * @brief �ӿ�Carnival: �� RenderWindow ������ Activity �Ĺ����ࡣ
*/
class ICarnival {
public:
	/**
	 * @brief ��Ϣ��Ķ�����Ϣ��
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
	 * @brief ��ʼִ�С�
	*/
	virtual void run() = 0;

	/**
	 * @brief ���Ƕ��� Activity �õ��˳����еķ�����
	*/
	void cancelKeepRunning();

	/**
	 * @brief �� Activity �õ����ñ�Ǩ�ķ�����
	 * @param t ��Ǩ���͡�
	*/
	void setTransition(Transition t);

	/**
	 * @brief ȡ�ø� Carnival ����� RenderWindow��
	*/
	sf::RenderWindow& getRenderWindow();

	/**
	 * @brief ��ʾһ����Ϣ��
	 * @param title ��Ϣ��ı��⡣
	 * @param text ��Ϣ������ݡ�
	 * @param info ��Ϣ��Ķ�����Ϣ��
	*/
	virtual void showMessageBox(std::string_view title, std::string_view text, MBInfo info = MBInfo::None) const;

	/**
	 * @brief �����������ڵĹرհ�ť�Ƿ����á�
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
