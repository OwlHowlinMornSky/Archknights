#include "Factory.h"

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include "../GUI_Activities/ActivityIDs.h"
#include "../GUI_Activities/Act01_DefaultEntry.h"
#include "../GUI_Activities/Act02_TestActivity.h"
#include "../GUI_Activities/Act03_Opening.h"
#include "../GUI_Activities/Act04_Load.h"

namespace Activity::Factory {

std::unique_ptr<GUI::IActivity> CreateActivity(uint32_t id) noexcept {
	try {
		switch (id) {
		case Activity::ID_DefaultEntry: // Ĭ����ڡ�
			return std::make_unique<Activity::Act01_DefaultEntry>();
#ifdef _DEBUG
		case Activity::ID_Test: // ���ԡ�
			return std::make_unique<Activity::Act02_TestActivity>(id);
			break;
#endif
		case Activity::ID_Opening: // �������档
			return std::make_unique<Activity::Act03_Opening>();
		case Activity::ID_Load: // ���ؽ��档
			return std::make_unique<Activity::Act04_Load>();
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
