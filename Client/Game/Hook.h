/*
*    Archknights
*
*    Copyright (C) 2023-2024  Tyler Parret True
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

#include "TypeDef.h"

namespace Game {

class Entity;

struct EntityMsg {
	MsgIdType msg;
	MsgWparamType wparam;
	MsgLparamType lparam;

	EntityMsg(
		MsgIdType _msg,
		MsgWparamType _wparam,
		MsgLparamType _lparam
	) :
		msg(_msg),
		wparam(_wparam),
		lparam(_lparam) {}
};

class Hook {
public:
	Hook();

	int callHook(Entity& entity, EntityMsg& msg);

protected:
	virtual int HookProc(Entity& entity, EntityMsg& msg) = 0;

	bool m_running;
};
// 广播消息将不会通过Hook.

}
