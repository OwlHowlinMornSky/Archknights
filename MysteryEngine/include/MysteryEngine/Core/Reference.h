﻿/*
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
#pragma once

#include <MysteryEngine/Core/MEAPI.h>

namespace ME {

template<class _T>
class ME_API Reference final {
public:
	Reference() :
		ref(nullptr) {}

	Reference(_T& r) :
		ref(&r) {}

	Reference& operator=(_T& r) {
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

	const _T* operator->() const {
		return ref;
	}

protected:
	_T* ref;
}; // template class Reference

} // namespace ME
