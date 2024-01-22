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
#include "Scene_GameCommon.h"

#include "../Game/IActorFactory.h"
#include "../Game/Animation.h"
#include "../Game/Model.h"

namespace gamegui {

Scene_GameCommon::Scene_GameCommon() {}

Scene_GameCommon::~Scene_GameCommon() {}

void Scene_GameCommon::update(float dt) {
}

bool Scene_GameCommon::handleEvent(const sf::Event& evt) {
    return false;
}

void Scene_GameCommon::AddAnimation(std::shared_ptr<game::Animation> a) {}

void Scene_GameCommon::AddModel(std::shared_ptr<game::Model> m) {}

size_t Scene_GameCommon::RegisterFactory(std::unique_ptr<game::IActorFactory> f) {
	return size_t();
}

bool Scene_GameCommon::AddActorFromFactory(size_t id) {
	return false;
}

void Scene_GameCommon::Update(float dt) {}

} // namespace gamegui
