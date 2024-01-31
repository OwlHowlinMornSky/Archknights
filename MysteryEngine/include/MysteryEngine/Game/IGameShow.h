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

#include <memory>

namespace ME {

class Animation;
class Model;
class IActorFactory;

class IGameShow {
public:
	IGameShow() = default;
	virtual ~IGameShow() = default;

public:
	virtual void AddAnimation(std::shared_ptr<Animation> a) = 0;
	virtual void AddModel(std::shared_ptr<Model> m) = 0;

	virtual size_t RegisterFactory(std::unique_ptr<IActorFactory> f) = 0;
	virtual bool AddActorFromFactory(size_t id) = 0;

	virtual void Update(float dt) = 0;
};

} // namespace game
