/*
*                    GNU AFFERO GENERAL PUBLIC LICENSE
*                       Version 3, 19 November 2007
*
*    Copyright (c) 2023  Tyler Parret True
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
*     Tyler Parret True (OwlHowlinMornSky) <mysteryworldgod@outlook.com>
*/
#pragma once

#include <string>

namespace Audio {

/**
 * @brief �ӿ� Bgm: �����ر���������ļ����á�
*/
class IBgm {
public:
	enum class Status {
		Stopped,
		Paused,
		Playing
	};

public:
	IBgm() = default;
	virtual ~IBgm() = default;

public:
	/**
	 * @brief ���ļ���
	 * @param filename: �ļ��������·�������·������
	 * @return ���Ƿ�ɹ���
	*/
	virtual [[nodiscard]] bool openFromFile(std::string_view filename) = 0;

	/**
	 * @brief ���š���ֹͣ�Ļ��ͷ��ʼ����ͣ�Ļ������
	*/
	virtual void play() = 0;
	/**
	 * @brief ��ͣ���ٴ� play ʱ������
	*/
	virtual void pause() = 0;
	/**
	 * @brief ֹͣ���ٴ� play ʱ��ͷ��ʼ��
	*/
	virtual void stop() = 0;
	/**
	 * @brief ��ȡ��ǰ״̬��
	 * @return ״̬��
	*/
	virtual Status getStatus() const = 0;

	/**
	 * @brief ��ȡ��ǰ������
	 * @return ������0 ~ 100����
	*/
	virtual float getVolume() const = 0;
	/**
	 * @brief ����������
	 * @param volume: ������0 ~ 100����
	*/
	virtual void setVolume(float volume) = 0;

	/**
	 * @brief ��ȡ��ǰ���ŵ�λ�á�
	 * @return λ�ã��룩��
	*/
	virtual float getTime() const = 0;
	/**
	 * @brief ���ò���λ�á�
	 * @param seconds: �µ�λ�ã��룩��
	*/
	virtual void setTime(float seconds) = 0;
	/**
	 * @brief ��ȡ��ʱ����
	 * @return ��ʱ�����룩��
	*/
	virtual float getLength() const = 0;
}; // class I_BGM

} // namespace Audio
