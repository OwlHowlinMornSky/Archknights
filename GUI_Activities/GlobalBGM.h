#pragma once

#include <memory>
#include <string>
#include <queue>
#include <mutex>

#include "../GUI/IBgm.h"

namespace Activity::Global {

class BGM final {
protected:
	BGM();
public:
	~BGM();

public:
	bool setup(); // 在DefaultEvtrance使用的
	void drop();

	void play(std::string_view file);
	void stop();

public:
	static BGM instance;

protected:
	void run();

	void stopOne();

protected:
	std::atomic_bool m_run, m_running;
	std::unique_ptr<GUI::IBgm> m_bgm;
	std::queue<std::string> m_cmds;
	std::mutex m_cmdMutex;
};

}
