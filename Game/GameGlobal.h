﻿#pragma once

#include <memory>
#include <string>

namespace game {

class GameBoard;

struct GlobalData {
	std::string name;
	std::unique_ptr<GameBoard> board;
};

class Global {
protected:
	Global() = default;
public:
	~Global() = default;

public:
	static Global* instance();
	static void drop();

public:
	GlobalData data;
};

}
