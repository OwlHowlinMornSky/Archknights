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
		None,
		Info,
		Error
	};

public:
	ICarnival(sf::RenderWindow* r_window);

	virtual ~ICarnival();

public:
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
	sf::RenderWindow* ref_window;
}; // class ICarnival

} // namespace GUI
