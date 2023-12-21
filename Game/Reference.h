#pragma once

namespace game {

template<class _T>
class Reference final {
	using _this_T = Reference;
public:
	Reference() noexcept :
		m_ref(nullptr) {}

	Reference(_T& r) noexcept :
		m_ref(&r) {}

	_this_T& operator=(_T& r) {
		m_ref = &r;
		return *this;
	}

	_this_T& operator()() {
		m_ref = nullptr;
		return *this;
	}

	_this_T& operator()(_T& r) {
		m_ref = &r;
		return *this;
	}

	_T* operator->() {
		return m_ref;
	}

protected:
	_T* m_ref;
};

}
