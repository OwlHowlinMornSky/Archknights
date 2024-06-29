/*
*    Archknights
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

#include <MysteryEngine/G3D/IModel.h>
#include <memory>

namespace Model {

class IAnimationPose {
public:
	IAnimationPose() = default;
	virtual ~IAnimationPose() = default;

public:
	virtual std::shared_ptr<ME::IModel> CreateAnimation() = 0;
};

class IAnimationFactory {
public:
	IAnimationFactory() = default;
	virtual ~IAnimationFactory() = default;

public:
	/**
	 * @brief 
	 * @param ptr 
	 * @param name 角色名称（非文件名称，例如基建动画文件名多一个‘build_’，但传参不应包含）。
	 * @param type 0: 战斗正面, 1: 战斗背面, 2: 基建.
	 * @return true 则加载成功。
	 */
	virtual bool CreatePose(
		std::unique_ptr<IAnimationPose>& ptr,
		std::string_view name,
		unsigned char type
	) = 0;
	/**
	 * @brief 
	 * @param ptr0 
	 * @param ptr1 
	 * @param name 
	 * @return 0: 都加载失败, 1: ptr0加载成功, 2: ptr1加载成功, 3: 都加载成功.
	 */
	virtual char CreatePose2(
		std::unique_ptr<IAnimationPose>& ptr0,
		std::unique_ptr<IAnimationPose>& ptr1,
		std::string_view name
	) = 0;
	/**
	 * @brief
	 * @param ptr
	 * @param name 角色名称（非文件名称，例如基建动画文件名多一个‘build_’，但传参不应包含）。
	 * @param type 0: 战斗正面, 1: 战斗背面, 2: 基建.
	 * @return true 则加载成功。
	 */
	virtual bool CreateEnemyPose(
		std::unique_ptr<IAnimationPose>& ptr,
		std::string_view name
	) = 0;

public:
	static IAnimationFactory* Instance();
	static void Drop();
};

}
