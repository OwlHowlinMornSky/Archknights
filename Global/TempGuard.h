/*
*    Archknights
*
*    Copyright (C) 2023  Tyler Parret True
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

namespace ohms {

template<class _T>
class TempGuard final {
public:
	TempGuard(_T& t) noexcept :
		m_target(t) {
		m_old = t;
	}
	TempGuard(const TempGuard&) = delete;
	TempGuard(TempGuard&&) = delete;
	~TempGuard() noexcept {
		m_target = m_old;
	}
	TempGuard& operator=(const TempGuard&) = delete;
	void operator=(const _T& n) noexcept {
		m_target = n;
	}
	void operator=(_T&& n) noexcept {
		m_target = n;
	}
protected:
	_T& m_target;
	_T m_old;
};

}
