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

/**
 * @brief ��ʱ��ȡ����Ȩ��������Ŀ��ľ���ֵ������ʱ�Զ���ԭ��ֵ��
 * @tparam _T: �κο������쳣���Ƶ����͡�
*/
template<class _T>
class TempGuard final {
public:
	/**
	 * @brief ���캯�����Զ����Ʊ�����ֵ��
	 * @param t: Ŀ�����
	*/
	explicit TempGuard(_T& t) noexcept :
		m_target(t) {
		m_old = t;
	}
	/**
	 * @brief �����������Զ���ԭ��ֵ��
	*/
	~TempGuard() noexcept {
		m_target = m_old;
	}

	TempGuard(const TempGuard&) = delete;
	TempGuard& operator=(const TempGuard&) = delete;

	/**
	 * @brief ��ʱ�޸ġ���Ŀ���ֵ��Ϊn��
	 * @param n: ��ʱֵ��
	*/
	void operator=(const _T& n) noexcept {
		m_target = n;
	}
	/**
	 * @brief ��ʱ�޸ġ���Ŀ���ֵ��Ϊn��
	 * @param n: ��ʱֵ��
	*/
	void operator=(_T&& n) noexcept {
		m_target = n;
	}

protected:
	_T& m_target;
	_T m_old;
};

}
