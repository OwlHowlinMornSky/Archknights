#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <MysteryEngine/Core/MEAPI.h>

namespace glm {

template struct ME_API vec<2, float, defaultp>;
template struct ME_API vec<3, float, defaultp>;
template struct ME_API vec<4, float, defaultp>;

template struct ME_API mat<2, 2, float, defaultp>;
template struct ME_API mat<3, 3, float, defaultp>;
template struct ME_API mat<4, 4, float, defaultp>;

}
