/*
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

/**
 * @brief 临时获取所有权，并保护目标的旧有值，析构时自动还原旧值。
 * @tparam _T: 任何可以无异常复制的类型。
*/
template<class _T>
class ME_API TempGuard final {
public:
	/**
	 * @brief 构造函数，自动复制保留旧值。
	 * @param t: 目标对象。
	*/
	explicit TempGuard(_T& t) noexcept :
		m_target(t) {
		m_old = t;
	}
	/**
	 * @brief 析构函数，自动还原旧值。
	*/
	~TempGuard() noexcept {
		m_target = m_old;
	}

	TempGuard(const TempGuard&) = delete;
	TempGuard& operator=(const TempGuard&) = delete;

	/**
	 * @brief 临时修改。把目标的值变为n。
	 * @param n: 临时值。
	*/
	void operator=(const _T& n) noexcept {
		m_target = n;
	}
	/**
	 * @brief 临时修改。把目标的值变为n。
	 * @param n: 临时值。
	*/
	void operator=(_T&& n) noexcept {
		m_target = n;
	}

protected:
	_T& m_target;
	_T m_old;
}; // template class TempGuard

} // namespace ME
