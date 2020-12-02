#pragma once

#include "../types.hpp"

#include <optional>

namespace meta_scene::objects::internal {

	struct homogeneous_medium final {
		spectrum sigma_a = vector3(0);
		spectrum sigma_s = vector3(0);

		real g = 0;
	};

	struct heterogeneous_medium final {
		matrix4x4 transform = matrix4x4(1);
		spectrum albedo = vector3(0);
		
		std::vector<real> sigma_t;
		
		uint32 size_x = 0;
		uint32 size_y = 0;
		uint32 size_z = 0;

		real g = 0;
	};
	
}

namespace meta_scene::objects {

	struct medium final {
		internal::heterogeneous_medium heterogeneous;
		internal::homogeneous_medium homogeneous;
		
		std::string type = "unknown";
	};

	struct media final {
		std::optional<medium> outside = std::nullopt;
		std::optional<medium> inside = std::nullopt;

		media() = default;
	};
	
}
