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

#include "TypeDef.h"
#include "Entity.h"

namespace Game {

struct Modifier {
	enum ModifyType : int {
		Grow = 0,
		Percent,
		Add,
		Times,

		COUNT
	};

	Attribute::ValueType value[ModifyType::COUNT];

	struct SourceType {
		enum EntityFunction {
			Normal = 0,
			Atk,
			Skill0,
			Skill1,
			Skill2,
			Skill3,
			Skill4,
			Skill5,
			Skill6,
			Skill7,
			Talent0,
			Talent1,
			Talent2,
			Talent3
		} func;
		std::weak_ptr<Entity> entity;

		SourceType() :
			entity(),
			func() {}

		SourceType(std::weak_ptr<Entity> _e, EntityFunction _f) :
			entity(_e),
			func(_f) {}
	} source;

	Modifier() :
		value() {}
};

class ModifySwitch final {
public:
	ModifySwitch();
	~ModifySwitch();

	/**
	 * @brief 修改修饰器来源。如果修饰器正生效，该函数*不会*使之失效。
	 * @param _ad
	 * @param _id
	 * @param _f
	 */
	void setSource(std::weak_ptr<Entity> _e, Modifier::SourceType::EntityFunction _f);

	/**
	 * @brief 修改修饰目标。如果修饰器正生效，该函数*会*使之失效。
	 * @param targetAd
	 * @param targetId
	 * @param attrib
	 */
	void setTarget(std::weak_ptr<Entity> target, Entity::AttributeType attrib);

	/**
	 * @brief 修改修饰值。如果修饰器正生效，该函数*不会*使之失效。
	 * @param grow
	 * @param percent
	 * @param add
	 * @param times
	 */
	void setValue(Attribute::ValueType grow, Attribute::ValueType percent, Attribute::ValueType add, Attribute::ValueType times);
	
	bool setEnabled(bool enabled);
	bool getEnabled() const;

protected:
	bool m_enabled;
	Entity::AttributeType m_attribute;
	std::weak_ptr<Entity> m_target;
	std::list<Modifier>::iterator m_handle;
	Modifier m_modifier;
};

}

