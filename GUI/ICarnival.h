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

	/**
	 * @brief ������ջ�����ͣ�����Ϊ Pause������Ϊ Stop��0 ��Ч��
	*/
	enum Transition : int {
		Switch = 1, // 1 ��Ŀ�ꡣ��ǰ Activity �ᱻ������Ŀ�꽫��ѹ��ջ�����С�
		Push      , // 1 ��Ŀ�ꡣ�佫��ѹ��ջ�в����С�
		Pop       , // 0 ��Ŀ�ꡣ��ǰ Activity �ᱻ�������µ�ջ�������У���ջ�����˳���
		PopTo     , // 1 ��Ŀ�ꡣ���Ŀ����ջ�У���ջ�ᱻ����Ŀ�꣬Ŀ����ջ���ҽ����У�������Ч����
		PopPush   , // 2 ��Ŀ�ꡣ���Ŀ��0��ջ�У���ջ�ᱻ����֮��Ŀ��1��ѹ���ҽ����У������� Push ��ͬ��
		Exit        // 0 ��Ŀ�ꡣ��� Ǩ������ Ϊ Pause�������˳�����Ϊ Stop����Ϊǿ���˳���
	};

public:
	ICarnival();

	virtual ~ICarnival();

public:
	/**
	 * @brief ��ʼִ�С�
	*/
	virtual void run() = 0;

	/**
	 * @brief ���Ƕ��� Activity �õ��˳����еķ�����
	*/
	virtual void cancelKeepRunning() = 0;

	/**
	 * @brief �� Activity �õ����ñ�Ǩ�ķ�����
	 * @param t ��Ǩ���͡�
	*/
	virtual void setTransition(int t, size_t a0 = 0, size_t a1 = 0) = 0;

	/**
	 * @brief ȡ�ø� Carnival ����� RenderWindow��
	*/
	virtual sf::RenderWindow& getRenderWindow() = 0;

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
}; // class ICarnival

} // namespace GUI
