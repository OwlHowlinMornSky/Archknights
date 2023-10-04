/*
*                    GNU AFFERO GENERAL PUBLIC LICENSE
*                       Version 3, 19 November 2007
*
*    Copyright (c) 2023  Tyler Parret True
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
*     Tyler Parret True (OwlHowlinMornSky) <mysteryworldgod@outlook.com>
*
* @Description
*     ����һЩ�����Ļص���������ر�����
*/
#pragma once

#include <functional>

namespace Callbacks {

/**
 * @brief ���ڸı��Сʱ�Ļص���
 * @brief ֻ��Ҫ�� main ���������úã�֮��Ͳ����ٶ��ˡ�
 * @brief ���� MyWndProc ��ʹ�á�
*/
extern std::function<void()> OnSizing;
/**
 * @brief ����״̬�Ļص���
 * @brief ��������ڵ�Ŀ����Ҫ���Ƿ�ֹ �ƶ����� �� �ı䴰�ڴ�С ʱ �����߼���ס��
 * @brief ������ ���ö�ʱ������ʱ���Ļص� MyTimerProc �����������⡣
 * @brief ���������߼���Ǩ��ʱ��Ҫ���������������һ���ǵøĻ�������Ϊ�г�ʼ�պ�������
*/
extern std::function<void()> OnIdle;
/**
 * @brief ����ϵͳѭ��ʱ�Ļص���
 * @brief �������������� ��ʼ�ƶ����� ���� ��ʼ�ı䴰�ڴ�С ʱ���õġ�
 * @brief ����֪ͨ�����һЩ����������ͣ��Ϸʲô�ģ���
 * @brief ���� �˳��ƶ���ı��С ��ʱ�� ͻȻ����һ�� �������� ������ʮ�� �� ֡��
*/
extern std::function<void()> OnEnterSysloop;
/**
 * @brief �˳�ϵͳѭ��ʱ�Ļص���
 * ͬ�ϣ�ֻ������ �˳�ϵͳѭ�� �ָ����� ʱ ���á�
*/
extern std::function<void()> OnExitSysloop;

/**
 * @brief ���� ���� ��ǰ���ڵ� �رհ�ť �Ƿ����á�
 * @brief ��Ӵ˱�����ԭ�� �� �� MyWndProc���� �� ���� WM_NCLBUTTONUP:HTCLOSE ʱ ����֪����״̬��
 * @brief ���� ϵͳ���ļ�� ̫�����ӣ�����ֱ�� �ñ�������������
 * @brief �� CarnivalWin32 �� ���úͼ�鰴ť״̬�ķ����� Ҳ���õ������
*/
extern bool ButtonEnabled_Close;

} // namespace Callbacks
