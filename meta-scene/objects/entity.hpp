#pragma once

#include "material.hpp"
#include "medium.hpp"
#include "light.hpp"
#include "shape.hpp"

#include <optional>

namespace meta_scene::objects {

	struct entity final {
		matrix4x4 transform = matrix4x4(1);
		
		std::optional<material> material;
		std::optional<light> light;
		std::optional<shape> shape;
		std::optional<media> media;
	};
	
}
