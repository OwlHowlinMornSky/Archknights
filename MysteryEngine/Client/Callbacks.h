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

#include <SFML/Config.hpp>

#ifdef SFML_SYSTEM_WINDOWS

#include <functional>

namespace ME {

/**
 * @brief 闲置状态的回调。
 * @brief 这玩意存在的目的主要就是防止 移动窗口 和 改变窗口大小 时 把主逻辑卡住。
 * @brief 方法是 设置定时器，定时器的回调 MyTimerProc 会调用这个玩意。
 * @brief 所以在主逻辑变迁的时候要改这个，不过用完一定记得改回来（因为有初始空函数）。
*/
extern std::function<void()> OnIdle;
/**
 * @brief 进入或退出系统循环时的回调。参数为 true 则为 进入，false 为退出。
 * @brief 如上所述，就是 开始移动窗口 或者 开始改变窗口大小 时调用的。
 * @brief 可以通知你进行一些处理（比如暂停游戏什么的），
 * @brief 避免 退出移动或改变大小 的时候 突然出现一个 长达数秒 甚至数十秒 的 帧。
*/
extern std::function<void(bool)> OnSystemLoop;

} // namespace ME

#endif
