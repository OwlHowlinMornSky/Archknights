#include "GlobalBGM.h"
#include "../GUI/BgmSFML.h"
#include <thread>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Clock.hpp>

namespace Activity::Global {

BGM BGM::instance;

BGM::BGM() :
	m_run(false),
	m_running(false) {}

BGM::~BGM() {
	drop();
}

bool BGM::setup() {
	if (m_run || m_running)
		return true;
	m_run = true;
	while (!m_cmds.empty())
		m_cmds.pop();
	std::thread th(&BGM::run, this);
	th.detach();
	return !th.joinable();
}

void BGM::drop() {
	m_run = false;
	while (m_running)
		sf::sleep(sf::milliseconds(30));
}

void BGM::play(std::string_view file) {
	std::lock_guard lg(m_cmdMutex);
	m_cmds.push(std::string("p:").append(file));
}

void BGM::stop() {
	std::lock_guard lg(m_cmdMutex);
	m_cmds.push("s;");
}

void BGM::run() {
	m_running = true;
	m_bgm = std::make_unique<GUI::BgmSFML>();
	while (m_run) {
		bool res;
		std::string str;
		{
			std::lock_guard lg(m_cmdMutex);
			res = m_cmds.empty();
			if (!res) {
				str = m_cmds.front();
				m_cmds.pop();
			}
		}
		if (!res) {
			switch (str[0]) {
			case 's':
				if (m_bgm->getStatus() != GUI::IBgm::Status::Playing)
					break;
				stopOne();
				break;
			case 'p':
				if (m_bgm->getStatus() == GUI::IBgm::Status::Playing)
					stopOne();
				if (m_run) {
					m_bgm->openFromFile(str.substr(2));
					m_bgm->setVolume(100.0f);
					m_bgm->play();
				}
				break;
			}
		}
		else {
			sf::sleep(sf::milliseconds(30));
		}
	}
	m_bgm->stop();
	m_bgm.reset();
	m_running = false;
	return;
}

void BGM::stopOne() {
	static sf::Time t = sf::milliseconds(200);
	sf::Clock clk;
	clk.restart();
	while (clk.getElapsedTime() < t && m_run) {
		m_bgm->setVolume(100.0f * (1.0f - clk.getElapsedTime() / t));
		sf::sleep(sf::milliseconds(10));
	}
	m_bgm->stop();
}

}
