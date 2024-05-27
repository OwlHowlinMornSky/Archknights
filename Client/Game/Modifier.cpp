#include "Modifier.h"

#include "GameGlobal.h"
#include "GameBoard.h"

#include "BasicMsgId.h"
#include "MsgResult.h"

Game::ModifySwitch::ModifySwitch() :
	m_enabled(false),
	m_targetAd(0),
	m_targetId(0) {}

Game::ModifySwitch::~ModifySwitch() {
	SetEnabled(false);
}

void Game::ModifySwitch::SetTarget(EntityLocationType targetAd, EntityIdType targetId) {
	SetEnabled(false);
	m_targetAd = targetAd;
	m_targetId = targetId;
}

void Game::ModifySwitch::SetModifier(Modifier modifer) {
	SetEnabled(false);
	modifer = modifer;
}

void Game::ModifySwitch::SetValue(Attribute::ValueType value) {
	bool prevEnabled = m_enabled;
	SetEnabled(false);
	m_modifier.value = value;
	SetEnabled(prevEnabled);
}

void Game::ModifySwitch::SetEnabled(bool enabled) {
	if (m_targetId == 0) {
		m_enabled = false;
		return;
	}
	if (enabled != m_enabled) {
		if (enabled) {
			auto res =
				GameGlobal::board->TellMsg(m_targetAd, m_targetId, MsgId::OnSetModify, 0, (intptr_t)&m_modifier);
			if (res == MsgResult::OK) {
				m_enabled = true;
			}
		}
		else {
			auto res =
				GameGlobal::board->TellMsg(m_targetAd, m_targetId, MsgId::OnCancelModify, 0, (intptr_t)&m_modifier);
			if (res == MsgResult::OK) {
				m_enabled = false;
			}
		}
	}
}
