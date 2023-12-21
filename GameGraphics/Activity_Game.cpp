#include "Activity_Game.h"

game::Activity_Game::Activity_Game() {}

game::Activity_Game::~Activity_Game() noexcept {}

bool game::Activity_Game::start(GUI::Window& wnd) noexcept {
	r(wnd);

	
    return false;
}

void game::Activity_Game::stop() noexcept {
	r();
}

bool game::Activity_Game::handleEvent(const sf::Event& evt) {
	return false;
}

void game::Activity_Game::update(sf::Time dtime) {}

void game::Activity_Game::OnEnterSysloop() noexcept {}

void game::Activity_Game::OnExitSysloop() noexcept {}
