/*
*    Mystery Engine
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
#include <MysteryEngine/Client/GlobalBGM.h>

#ifdef USE_ASYNC_MUSIC

#include <memory>
#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Clock.hpp>

namespace {

struct GlobalBGMdata {
	std::atomic_bool m_run, m_running;
	std::unique_ptr<ME::Bgm> m_bgm;
	std::queue<std::string> m_cmds;
	std::mutex m_cmdMutex;

	GlobalBGMdata() :
		m_run(false),
		m_running(false) {}
};

GlobalBGMdata* data = nullptr;

void stopOne() {
	static sf::Time t = sf::milliseconds(200);
	sf::Clock clk;
	clk.restart();
	while (clk.getElapsedTime() < t && data->m_run) {
		data->m_bgm->setVolume(100.0f * (1.0f - clk.getElapsedTime() / t));
		sf::sleep(sf::milliseconds(10));
	}
	data->m_bgm->stop();
}

void run() {
	data->m_running = true;
	data->m_bgm = ME::CreateBGM();
	while (data->m_run) {
		bool res;
		std::string str;
		{
			std::lock_guard lg(data->m_cmdMutex);
			res = data->m_cmds.empty();
			if (!res) {
				str = data->m_cmds.front();
				data->m_cmds.pop();
			}
		}
		if (!res) {
			switch (str[0]) {
			case 's':
				if (data->m_bgm->getStatus() != ME::Bgm::Status::Playing)
					break;
				stopOne();
				break;
			case 'p':
				if (data->m_bgm->getStatus() == ME::Bgm::Status::Playing)
					stopOne();
				if (data->m_run) {
					data->m_bgm->openFromFile(str.substr(2));
					data->m_bgm->setVolume(100.0f);
					data->m_bgm->play();
				}
				break;
			}
		}
		else {
			sf::sleep(sf::milliseconds(80));
		}
	}
	data->m_bgm->stop();
	data->m_bgm.reset();
	data->m_running = false;
	return;
}

} // namespace

namespace ME::GlobalBGM {

bool GlobalBGM::setup() {
	if (data)
		return true;
	data = new GlobalBGMdata;
	data->m_run = true;
	while (!data->m_cmds.empty())
		data->m_cmds.pop();
	std::thread th(&::run);
	th.detach();
	return !th.joinable();
}

void GlobalBGM::drop() {
	data->m_run = false;
	while (data->m_running) {
		sf::sleep(sf::milliseconds(30));
	}
	delete data;
}

void GlobalBGM::play(std::string_view file) {
	std::lock_guard lg(data->m_cmdMutex);
	data->m_cmds.push(std::string("p:").append(file));
}

void GlobalBGM::stop() {
	std::lock_guard lg(data->m_cmdMutex);
	data->m_cmds.push("s;");
}

} // namespace ME::GlobalBGM

#else

namespace {

std::unique_ptr<ME::Bgm> m_bgm;

} // namespace

namespace ME::GlobalBGM {

bool GlobalBGM::setup() {
	if (::m_bgm)
		return true;
	::m_bgm = ME::CreateBGM();
	return !::m_bgm;
}

void GlobalBGM::drop() {
	::m_bgm.reset();
}

void GlobalBGM::play(std::string_view file) {
	::m_bgm->openFromFile(file);
	::m_bgm->play();
}

void GlobalBGM::stop() {
	::m_bgm->stop();
}

} // namespace ME::GlobalBGM

#endif
