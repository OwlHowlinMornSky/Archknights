#pragma once

namespace ohms {

template<class _T>
class Ref final {
public:
	Ref() :
		ref(nullptr) {}

	Ref(_T& r) :
		ref(&r) {}

	Ref& operator=(_T& r) {
		ref = &r;
		return *this;
	}

	void operator()() {
		ref = nullptr;
	}

	void operator()(_T& r) {
		ref = &r;
	}

	_T* operator->() {
		return ref;
	}

protected:
	_T* ref;
};

}
