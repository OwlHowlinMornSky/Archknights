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

#include <memory>
#include <list>

#include <MysteryEngine/G3D/IModel.h>
#include <MysteryEngine/G3D/Scene.h>
#include "IActor.h"

namespace Game {

class Stage :
	public ME::Scene {
protected:
	Stage();
public:
	virtual ~Stage() = default;

public:
	virtual void AddGround(std::shared_ptr<ME::IModel> ground) = 0;
	virtual void AddActor(std::shared_ptr<IActor> actor) = 0;

	virtual void SetGroundSize(float x, float y) = 0;
};

} // namespace Game
