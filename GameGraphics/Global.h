#pragma once

#include <string>

namespace gamegui {

struct GlobalData {
	std::string name;
};

class Global final {
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
