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

	int CallHook(Entity& entity, EntityMsg& msg);

protected:
	virtual int HookProc(Entity& entity, EntityMsg& msg) = 0;

	bool m_running;
};
// 广播消息将不会通过Hook.

}
