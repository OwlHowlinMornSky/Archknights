/*
*    Archknights
*
*    Copyright (C) 2023  Tyler Parret True
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published
*    by the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* @Authors
*    Tyler Parret True <mysteryworldgod@outlook.com><https://github.com/OwlHowlinMornSky>
*/
#pragma once

#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

namespace GUI {

/**
* @brief ������ջ�����ͣ�����Ϊ Stop������Ϊ Pause��0 ��Ч��
*/
namespace Transition {
enum : int {
	Switch = 1, // 1 ��Ŀ�ꡣ��ǰ Activity �ᱻ������Ŀ�꽫��ѹ��ջ�����С�
	Push,       // 1 ��Ŀ�ꡣ�佫��ѹ��ջ�в����С�
	Pop,        // 0 ��Ŀ�ꡣ��ǰ Activity �ᱻ�������µ�ջ�������У���ջ�����˳���
	PopTo,      // 1 ��Ŀ�ꡣ���Ŀ����ջ�У���ջ�ᱻ����Ŀ�꣬Ŀ����ջ���ҽ����У�������Ч����
	PopPush,    // 2 ��Ŀ�ꡣ���Ŀ��0��ջ�У���ջ�ᱻ����֮��Ŀ��1��ѹ���ҽ����У������� Push ��ͬ��
	Exit        // 0 ��Ŀ�ꡣǿ���˳���
};
} // namespace Transition

/**
* @brief ��Ϣ��Ķ�����Ϣ��
*/
enum class MBInfo {
	None = 0,
	Info,
	Error
};

enum class WindowType {
	Windowed = 0, // ���ڻ���
	Borderless,   // �ޱ߿�
	Fullscreen    // ȫ����
};

/**
 * @brief �ӿ�Carnival: �� RenderWindow ������ Activity �Ĺ����ࡣ
*/
class ICarnival {
public:
	ICarnival() noexcept = default;
	virtual ~ICarnival() = default;

/**
 * @brief �������������ϵͳ�йء�
*/
public:
	/**
	 * @brief һ��ϵͳ����Ϣѭ�������������ڼ���ʱ���ⴰ�ڱ��ж�Ϊδ��Ӧ��
	 * @param callerDoWantToHandleThem: ���Ϊ true��������߻�Ӧ��ʹ�� SFML ����Ϣѭ��������Ϣ��
	 * ����ú���ͬʱ�� SFML ����Ϣ������ա�
	*/
	virtual void systemMessagePump(bool callerDoWantToHandleThem) const noexcept = 0;

	/**
	 * @brief ��ʾһ����Ϣ��
	 * @param title: ��Ϣ��ı��⡣
	 * @param text: ��Ϣ������ݡ�
	 * @param info: ��Ϣ��Ķ�����Ϣ��
	*/
	virtual void systemShowMessageBox(std::string_view title,
								std::string_view text,
								MBInfo info = MBInfo::None) const noexcept = 0;

	/**
	 * @brief ���� �������ڵ� �رհ�ť �Ƿ����á�
	 * @param enabled: True �����ã�������á�
	*/
	virtual void windowSetCloseEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief ���� �������ڵ� Resize �߿� �� ��󻯰�ť �Ƿ����á�
	 * @param enabled: True �����ã�������á�
	*/
	virtual void windowSetResizeEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief ���� �������ڵ� ��С����ť �Ƿ����á�
	 * @param enabled: True �����ã�������á�
	*/
	virtual void windowSetMinimizeEnabled(bool enabled) noexcept = 0;

	/**
	 * @brief ��Ϊһ�㴰�ڡ�
	*/
	virtual void windowSetWindowed() noexcept = 0;
	/**
	 * @brief ��Ϊ�ޱ߿򴰿ڡ�
	*/
	virtual bool windowSetBorderless() noexcept = 0;
	/**
	 * @brief ��Ϊȫ����
	 * @brief ������ֱ�������Ļ��һ�£���ᶪʧ���㣬�����ҵĻ�������������OHMS��
	 * @param w: ��
	 * @param h: �ߡ�
	*/
	virtual bool windowSetFullscreen(sf::VideoMode mode) noexcept = 0;

	/**
	 * @brief ����˯�߼��������˴���˵˯�߰����������ر���Ļ������
	*/
	virtual void systemResetSleepCounter() noexcept = 0;

	/**
	 * @brief ������ֹϵͳ˯�ߣ��˴���˵˯�߰����������ر���Ļ������
	 * @param allowSleep: �Ƿ�����˯�ߣ���Ϊ false �� ��ֹ˯�� �� ������Ļ������
	*/
	virtual void systemTrySetSleepEnabled(bool allowSleep) noexcept = 0;

/**
 * @brief ����������ϵͳ�޹أ���Carnival�����Ѿ�ʵ�֡�
*/
public:
	/**
	 * @brief ��ʼִ�С�
	*/
	virtual void run() noexcept = 0;

	/**
	 * @brief ȡ�ø� Carnival ����� RenderWindow��
	*/
	virtual sf::RenderWindow& getRenderWindow() noexcept = 0;

	/**
	 * @brief ���Ƕ��� Activity �õ��˳����еķ�����
	*/
	virtual void meDependentActivityStopRunning() noexcept = 0;
	/**
	 * @brief �� Activity �õ����ñ�Ǩ�ķ�����
	 * @param t: ��Ǩ���͡�
	*/
	virtual void meActivitySetTransition(int t, uint32_t a0 = 0, uint32_t a1 = 0) noexcept = 0;

	/**
	 * @brief �����Ƿ�� sizing ��Ϣ���� resized ��Ϣ�� Activity ����
	 * @param enabled: �Ƿ�����
	*/
	virtual void setSizingAsResized(bool enabled) noexcept = 0;
	/**
	 * @brief �Ƿ��Ѱ� sizing ��Ϣ���� resized ��Ϣ�� Activity ����
	 * @return �Ƿ�����
	*/
	virtual bool isSizingAsResized() const noexcept = 0;

	/**
	 * @brief ���ô��ڴ�С��˳�����View����
	 * @param w: ��
	 * @param h: �ߡ�
	*/
	virtual void windowSetClientSize(uint32_t w, uint32_t h) noexcept = 0;
	/**
	 * @brief ��� �������ڵ� �رհ�ť �Ƿ����á�
	 * @return True �������ã������ѽ��á�
	*/
	virtual bool windowIsCloseEnabled() const noexcept = 0;
	/**
	 * @brief ��� �������ڵ� Resize �߿� �� ��󻯰�ť �Ƿ����á�
	 * @return True �������ã������ѽ��á�
	*/
	virtual bool windowIsResizeEnabled() const noexcept = 0;
	/**
	 * @brief ��� �������ڵ� ��С����ť �Ƿ����á�
	 * @return True �������ã������ѽ��á�
	*/
	virtual bool windowIsMinimizeEnabled() const noexcept = 0;

	/**
	 * @brief ��ȡ��ǰ����״̬��
	 * @return ����״̬��
	*/
	virtual WindowType windowGetWindowType() const noexcept = 0;
}; // class ICarnival

} // namespace GUI
