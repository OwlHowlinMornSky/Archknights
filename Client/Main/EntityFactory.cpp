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
#include "EntityFactory.h"

namespace EntityFactoryLink {

std::unique_ptr<Main::EntityFactory> Create101();
std::unique_ptr<Main::EntityFactory> Create128();
std::unique_ptr<Main::EntityFactory> Create151();

std::unique_ptr<Main::EntityFactory> CreateEnemy1002();

}

bool Main::EntityFactory::Create(std::unique_ptr<Main::EntityFactory>& ptr, size_t entityId) {
	switch (entityId) {
	case 101:
		ptr = EntityFactoryLink::Create101();
		break;
	case 128:
		ptr = EntityFactoryLink::Create128();
		break;
	case 151:
		ptr = EntityFactoryLink::Create151();
		break;
	default:
		return false;
	}
    return true;
}

bool Main::EntityFactory::CreateEnemy(std::unique_ptr<Main::EntityFactory>& ptr, size_t entityId) {
	switch (entityId) {
	case 1002:
		ptr = EntityFactoryLink::CreateEnemy1002();
		break;
	default:
		return false;
	}
	return true;
}
