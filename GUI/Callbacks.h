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
*
* @Description
*     声明一些基本的回调函数和相关变量。
*/
#pragma once

#include <functional>

namespace Callbacks {

/**
 * @brief 正在改变大小时的回调。
 * @brief 只需要在 main 函数里设置好，之后就不用再动了。
 * @brief 会在 MyWndProc 里使用。
*/
extern std::function<void()> OnSizing;
/**
 * @brief 闲置状态的回调。
 * @brief 这玩意存在的目的主要就是防止 移动窗口 和 改变窗口大小 时 把主逻辑卡住。
 * @brief 方法是 设置定时器，定时器的回调 MyTimerProc 会调用这个玩意。
 * @brief 所以在主逻辑变迁的时候要改这个，不过用完一定记得改回来（因为有初始空函数）。
*/
extern std::function<void()> OnIdle;
/**
 * @brief 进入系统循环时的回调。
 * @brief 如上所述，就是 开始移动窗口 或者 开始改变窗口大小 时调用的。
 * @brief 可以通知你进行一些处理（比如暂停游戏什么的），
 * @brief 避免 退出移动或改变大小 的时候 突然出现一个 长达数秒 甚至数十秒 的 帧。
*/
extern std::function<void()> OnEnterSysloop;
/**
 * @brief 退出系统循环时的回调。
 * 同上，只不过是 退出系统循环 恢复正常 时 调用。
*/
extern std::function<void()> OnExitSysloop;

/**
 * @brief 用于 保存 当前窗口的 关闭按钮 是否启用。
 * @brief 添加此变量的原因 是 在 MyWndProc方法 里 处理 WM_NCLBUTTONUP:HTCLOSE 时 必须知晓其状态。
 * @brief 由于 系统级的检查 太过复杂，所以直接 用变量保存下来。
 * @brief 在 CarnivalWin32 的 设置和检查按钮状态的方法里 也会用到这个。
*/
extern bool ButtonEnabled_Close;

} // namespace Callbacks
