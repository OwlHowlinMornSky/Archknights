#include "char_128_plosis_actor.h"

Game::Char_128_Plosis_Actor_Vanilla::Char_128_Plosis_Actor_Vanilla(
	std::shared_ptr<ohms::ISpineAnimation> _f,
	std::shared_ptr<ohms::ISpineAnimation> _b
) : ActorSpine2(_f, _b) {
	_f->setScale(0.65f);
	_b->setScale(0.65f);
}

void Game::Char_128_Plosis_Actor_Vanilla::TriggerAnimation(AnimationEvent type, Direction direction) {
	switch (type) {
	case AnimationEvent::Begin:
		m_current->setAnimation(0, "Start", false);
		m_current->addAnimation(0, "Idle", true, 0.0f);
		break;
	case AnimationEvent::Idle:
		if (m_direction != Direction::NotCare && direction != m_direction) {
			m_targetDirection = direction;
			if (m_currentFBDirection != ((static_cast<char>(m_targetDirection) & 0x02) != 0)) {
				m_target = GetAnimation(!m_currentFBDirection);
				m_target->setAnimation(0, "Idle", true);
			}
			else {
				m_current->setAnimation(0, "Idle", true);
			}
			m_isRolling = true;
		}
		else {
			m_current->setAnimation(0, "Idle", true);
		}
		break;
	case AnimationEvent::Die:
		InitDirection((static_cast<char>(m_targetDirection) & 0x01) ? Direction::FL : Direction::FR);
		m_current->setAnimation(0, "Die", false);
		break;
	}
}

Game::Char_128_Plosis_Actor_Epoque::Char_128_Plosis_Actor_Epoque(
	std::shared_ptr<ohms::ISpineAnimation> _f,
	std::shared_ptr<ohms::ISpineAnimation> _b
) : ActorSpine2(_f, _b) {
	_f->setScale(0.65f);
	_b->setScale(0.65f);
}

void Game::Char_128_Plosis_Actor_Epoque::TriggerAnimation(AnimationEvent type, Direction direction) {
	switch (type) {
	case AnimationEvent::Begin:
		m_current->setAnimation(0, "Start", false);
		m_current->addAnimation(0, "Idle", true, 0.0f);
		break;
	case AnimationEvent::Idle:
		if (m_direction != Direction::NotCare && direction != m_direction) {
			m_targetDirection = direction;
			if (m_currentFBDirection != ((static_cast<char>(m_targetDirection) & 0x02) != 0)) {
				m_target = GetAnimation(!m_currentFBDirection);
				m_target->setAnimation(0, "Idle", true);
			}
			else {
				m_current->setAnimation(0, "Idle", true);
			}
			m_isRolling = true;
		}
		else {
			m_current->setAnimation(0, "Idle", true);
		}
		break;
	case AnimationEvent::Die:
		InitDirection((static_cast<char>(m_targetDirection) & 0x01) ? Direction::FL : Direction::FR);
		m_current->setAnimation(0, "Die", false);
		break;
	}
}
