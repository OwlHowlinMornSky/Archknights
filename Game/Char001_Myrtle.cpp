#include "Char001_Myrtle.h"

#include <iostream>
#include <random>

namespace {

// 以随机值播种，若可能
//std::random_device r;
// 选择 0.4 与 0.9 间的随机数
//std::default_random_engine e1(r());
std::default_random_engine e1(233);
std::uniform_real_distribution<float> uniform_dist(0.4f, 0.9f);

}

namespace game {

Char001_Myrtle::Char001_Myrtle() {
	m_cnt = 0.0f;
	m_returnCnt = 0;

	m_doki = uniform_dist(e1);

	std::cout << "Myrtle Created" << std::endl; // for test
}

void Char001_Myrtle::OnJoined(size_t id, size_t location) {
	Parent::OnJoined(id, location);
	std::cout << "Operator Myrtel Standby!" << std::endl; // for test
}

void Char001_Myrtle::OnUpdate(float dt) {
	m_cnt += dt;
	while (m_cnt >= m_doki) {
		m_cnt -= m_doki;
		m_returnCnt++;
		// for test
		std::cout << "Heart Beat From: ID: " << m_id << ", ProductID: " << m_productID << std::endl;
	}
	while (m_returnCnt >= 5) {
		m_returnCnt -= 5;
		// for test
		std::cout << "Back Home: ID: " << m_id << ", ProductID: " << m_productID << std::endl;
		KickSelf();
	}
}

void Char001_Myrtle::OnKicking() {
	Parent::OnKicking();
}

void Char001_Myrtle::UnitReset() {
	m_cnt = 0.0f;
	m_returnCnt = 0;

	m_doki = uniform_dist(e1);

	std::cout << "Myrtle Reseted" << std::endl; // for test
}

}
