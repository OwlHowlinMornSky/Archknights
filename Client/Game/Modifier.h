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
		EntityLocationType ad;
		EntityIdType id;

		SourceType() :
			id(0),
			ad(0),
			func() {}

		SourceType(EntityLocationType _ad, EntityIdType _id, EntityFunction _f) :
			id(_id),
			ad(_ad),
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
	void SetSource(EntityLocationType _ad, EntityIdType _id, Modifier::SourceType::EntityFunction _f);

	/**
	 * @brief 修改修饰目标。如果修饰器正生效，该函数*会*使之失效。
	 * @param targetAd
	 * @param targetId
	 * @param attrib
	 */
	void SetTarget(EntityLocationType targetAd, EntityIdType targetId, Entity::AttributeType attrib);

	/**
	 * @brief 修改修饰值。如果修饰器正生效，该函数*不会*使之失效。
	 * @param grow
	 * @param percent
	 * @param add
	 * @param times
	 */
	void SetValue(Attribute::ValueType grow, Attribute::ValueType percent, Attribute::ValueType add, Attribute::ValueType times);
	
	bool SetEnabled(bool enabled);
	bool GetEnabled() const;

protected:
	bool m_enabled;
	Entity::AttributeType m_attribute;
	EntityLocationType m_targetAd;
	EntityIdType m_targetId;
	std::list<Modifier>::iterator m_handle;
	Modifier m_modifier;
};

}

