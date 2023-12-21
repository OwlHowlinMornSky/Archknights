#include "Activity_Game.h"
#include "Global.h"
#include "../Game/GameGlobal.h"
#include "../Game/GameBoard.h"

namespace gamegui {

Activity_Game::Activity_Game() {}

Activity_Game::~Activity_Game() noexcept {}

bool Activity_Game::start(GUI::Window& wnd) noexcept {
	r(wnd);
	game::Global::instance()->data.name = "dabug";
	game::Global::instance()->data.board = std::make_unique<game::GameBoard>();
	m_scene = std::make_unique<SceneCommon>();
	return true;
}

void Activity_Game::stop() noexcept {
	m_scene.reset();
	r();
	return;
}

bool Activity_Game::handleEvent(const sf::Event& evt) {

	return false;
}

void Activity_Game::update(sf::Time dtime) {
	m_scene->update(dtime.asMicroseconds());
	r->draw(*m_scene);
	r->display();
	return;
}

void Activity_Game::OnEnterSysloop() noexcept {}

void Activity_Game::OnExitSysloop() noexcept {}

}
