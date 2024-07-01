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

#include "../Main/EntityFactory.h"
#include "../Models/IAnimation.h"

#include <memory>
#include <memory_resource>

namespace Unit {

class Enemy_1002_nsabr_Factory final :
	public Main::EntityFactory {
public:
	Enemy_1002_nsabr_Factory();
	virtual ~Enemy_1002_nsabr_Factory() override;

	virtual bool load() override;
	virtual bool createEntity(std::shared_ptr<Game::Entity>& ptr) override;

protected:
	std::unique_ptr<Model::IAnimationPose> m_pose[2];
	std::pmr::unsynchronized_pool_resource m_pool;
	std::pmr::polymorphic_allocator<Enemy_1002_nsabr_Factory> m_alloc;
};

}
