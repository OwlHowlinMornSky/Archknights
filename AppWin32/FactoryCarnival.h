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
*    �����˴��� Carnival ʵ���ķ�����
*/
#pragma once

#include "../GUI/ICarnival.h"
#include "framework.h"

namespace AppWin32::Factory {

/**
 * @brief ���� Carnival ʵ���ķ������䴴������ CarnivalWin32��������װ��Ϊ�˱�����ɢͷ�ļ���
 * @param hWnd: Ҫ�����Ĵ��� �� ������� CarnivalWin32 ������Ҫ�����ݡ�
 * @return �����õ� Carnival ʵ����
*/
std::unique_ptr<GUI::ICarnival> crateCarnival(HWND hWnd);

} // namespace AppWin32