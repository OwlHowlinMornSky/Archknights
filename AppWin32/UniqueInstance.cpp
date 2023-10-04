#include "UniqueInstance.h"

#include <mutex>

#pragma data_seg("___OHMS_EUI")
std::mutex ___OHMS_EnsureUniqueInstance_Mutex;
bool ___OHMS_EnsureUniqueInstance_instanceExist = false;
#pragma data_seg()
#pragma comment(linker, "/SECTION:___OHMS_EUI,RWS")

bool AppWin32::uniqueInstance() {
	std::lock_guard<std::mutex> guard(___OHMS_EnsureUniqueInstance_Mutex);
	if (___OHMS_EnsureUniqueInstance_instanceExist) {
		return false;
	}
	___OHMS_EnsureUniqueInstance_instanceExist = true;
    return true;
}

void AppWin32::instanceExit() {
	std::lock_guard<std::mutex> guard(___OHMS_EnsureUniqueInstance_Mutex);
	___OHMS_EnsureUniqueInstance_instanceExist = false;
	return;
}
