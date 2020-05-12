#pragma once

#include "../utilities.hpp"

namespace metascene {

	namespace materials {

		enum class type : uint32 {
			unknown = 0, diffuse = 1, plastic = 2
		};
		
		struct material {
			type type = type::unknown;

			material() = default;

			material(const materials::type& type);
		};
		
	}
}
