#pragma once

#include "../types.hpp"

namespace meta_scene::objects::internal {

	struct gaussian_filter {
		real alpha = 2;
	};

	struct box_filter {
		
	};
	
}

namespace meta_scene::objects {
	
	struct filter final {
		real radius_x = 0.5;
		real radius_y = 0.5;

		internal::gaussian_filter gaussian;
		internal::box_filter box;
		
		std::string type = "unknown";
	};
	
}
