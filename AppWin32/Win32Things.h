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
*
* @Description
*     Win32Things.h : �����й�Win32�Ļ�����Ϊ��
*/
#pragma once

#include "framework.h"

namespace SystemThings {

/**
 * @brief ���ϵͳ����GetLastError����
 * @param lpszFunction: ��ʾ�ʡ����Ե�ʱ���֪��ʲô��������ʲô��
*/
void WinCheckError(LPCWSTR lpszFunction) noexcept;

/**
 * @brief ����Դ��ȡ��Ҫ���ַ�����
 * @param hInstance: ���̾����
*/
void InitString(HINSTANCE hInstance) noexcept;

/**
 * @brief ע�ᴰ���ࡣ��������ǰ�ȵ��������
 * @param hInstance: ���̾����
 * @return �����Ƿ�ɹ���
*/
bool MyRegisterClass(HINSTANCE hInstance) noexcept;

/**
 * @brief �������ڡ�����ǰ����ע�ᴰ���ࡣ
 * @brief ע�⣺ֻ�ܴ���һ�����ڣ�������ڵı���δ֪��
 * @param hInstance: ���̾����
 * @param nCmdShow: ShowWindow �õ��Ǹ���WinMain ���βο϶��С�
 * @param hWnd: [Out] ���ھ����
 * @return �����Ƿ�ɹ���
*/
bool MyCreateWindow(HINSTANCE hInstance, int nCmdShow, HWND& hWnd) noexcept;

/**
 * @brief ע�������ࡣ
 * @param hInstance: ���̾����
*/
void MyUnregisterClass(HINSTANCE hInstance) noexcept;

} // namespace SystemThings
