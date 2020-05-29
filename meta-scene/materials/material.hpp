#pragma once

#include "../interfaces/string_property.hpp"
#include "../utilities.hpp"

namespace metascene {

	namespace materials {

		enum class type : uint32 {
			unknown = 0, diffuse = 1,
			plastic = 2, glass = 3,
			metal = 4, uber = 5,
			mirror = 6, substrate = 7,
			translucent = 8, mixture = 9
		};
		
		struct material : interfaces::string_property {
			type type = type::unknown;

			material() = default;

			material(const materials::type& type);
		};
		
	}
}
