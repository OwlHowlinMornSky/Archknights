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
 * @brief 接口 Bgm: 播放特别构造的音乐文件所用。
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
	 * @brief 打开文件。
	 * @param filename: 文件名（相对路径或绝对路径）。
	 * @return 打开是否成功。
	*/
	virtual [[nodiscard]] bool openFromFile(std::string_view filename) = 0;

	/**
	 * @brief 播放。已停止的会从头开始，暂停的会继续。
	*/
	virtual void play() = 0;
	/**
	 * @brief 暂停。再次 play 时继续。
	*/
	virtual void pause() = 0;
	/**
	 * @brief 停止。再次 play 时从头开始。
	*/
	virtual void stop() = 0;
	/**
	 * @brief 获取当前状态。
	 * @return 状态。
	*/
	virtual Status getStatus() const = 0;

	/**
	 * @brief 获取当前音量。
	 * @return 音量（0 ~ 100）。
	*/
	virtual float getVolume() const = 0;
	/**
	 * @brief 设置音量。
	 * @param volume: 音量（0 ~ 100）。
	*/
	virtual void setVolume(float volume) = 0;

	/**
	 * @brief 获取当前播放的位置。
	 * @return 位置（秒）。
	*/
	virtual float getTime() const = 0;
	/**
	 * @brief 设置播放位置。
	 * @param seconds: 新的位置（秒）。
	*/
	virtual void setTime(float seconds) = 0;
	/**
	 * @brief 获取总时长。
	 * @return 总时长（秒）。
	*/
	virtual float getLength() const = 0;
}; // class I_BGM

} // namespace Audio
