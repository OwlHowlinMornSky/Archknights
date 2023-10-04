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

namespace GUI {

/**
 * @brief Activity ��ʶ��
*/
enum IDs : size_t {
	ID_None = 0ull, //------------// �ա�
	ID_DefaultEntry, //-----------// Ĭ����ڡ�
	ID_Load, //-------------------// ���ؽ��档
	ID_Title, //------------------// ������档
	ID_Main, //-------------------// �����档
	ID_Panel, //------------------// �նˡ�
	ID_Construction, //-----------// ������

	ID_CTRL_BASE_COUNT, //--------// [���] �������� ������

	ID_DynamicBlock = 0x0080, //--// [���] ��̬�������� ��ʼ��ǡ�

	ID_RESERVED_COUNT = 0x0400 //-// [���] ������ ����������֮�ϵ� ID ����ʹ�á�
};

}
