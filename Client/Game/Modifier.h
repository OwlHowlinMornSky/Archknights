#pragma once

#include "TypeDef.h"
#include "Entity.h"

namespace Game {

struct Modifier {
	Entity::AttributeType attribute;
	Attribute::ModifyType calcType;
	Attribute::ValueType value;

	Modifier() :
		attribute(Entity::AttributeType::COUNT),
		calcType(Attribute::ModifyType::COUNT),
		value(0) {}
};

class ModifySwitch final {
public:
	ModifySwitch();
	~ModifySwitch();

	void SetTarget(EntityLocationType targetAd, EntityIdType targetId);
	void SetModifier(Modifier modifer);
	void SetValue(Attribute::ValueType value);
	void SetEnabled(bool enabled);

protected:
	bool m_enabled;
	EntityLocationType m_targetAd;
	EntityIdType m_targetId;
	Modifier m_modifier;
};

}

