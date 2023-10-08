#include "Factory.h"

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include "../GUI_Activities/ActivityIDs.h"
#include "../GUI_Activities/Act00_TestActivity.h"
#include "../GUI_Activities/Act01_DefaultEntry.h"
#include "../GUI_Activities/Act02_Opening.h"
#include "../GUI_Activities/Act03_Load.h"

namespace Activity::Factory {

std::unique_ptr<GUI::IActivity> CreateActivity(uint32_t id) noexcept {
	try {
		switch (id) {
		case Activity::ID_None:
#ifdef _DEBUG
			return std::make_unique<Activity::TestActivity>(id);
#endif
			break;
		case Activity::ID_DefaultEntry:// Ĭ����ڡ�
			return std::make_unique<Activity::DefaultEntry>();
		case Activity::ID_Opening: // �������档
			return std::make_unique<Activity::Act02_Opening>();
		case Activity::ID_Load: // ���ؽ��档
			return std::make_unique<Activity::Act03_Load>();
		case Activity::ID_Title: // ������档
		case Activity::ID_Main: // �����档
		case Activity::ID_Panel: // �նˡ�
		case Activity::ID_Construction: // ������
		default:
#ifdef _DEBUG
			throw std::exception("Invalid Activity ID!");
#endif
			break;
		}
	}
#ifdef _DEBUG
	catch (std::exception& e) {
		std::cerr << "Exception: GUI::CarnivalWin32: createActivity failed!" << std::endl
			<< "    " << e.what() << std::endl;
	}
#endif // _DEBUG
	catch (...) {
#ifdef _DEBUG
		std::cerr << "Exception: GUI::CarnivalWin32: createActivity failed!" << std::endl
			<< "    Unknown Exception." << std::endl;
#endif // _DEBUG
	}
	return std::unique_ptr<GUI::IActivity>();
}

}
