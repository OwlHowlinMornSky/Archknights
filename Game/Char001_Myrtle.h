#pragma once

#include "Tower.h"
#include "CommonFactory.h"

namespace game {

class Char001_Myrtle final :
	public Tower {};

using Char001_Myrtle_Factory = CommonFactory<Char001_Myrtle>;

}
