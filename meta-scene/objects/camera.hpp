#pragma once

#include "../types.hpp"

namespace meta_scene::objects::internal {
	
	struct depth_of_field final {
		real focus = 1e6;
		real lens = 0;
	};

	struct perspective final {
		real fov = 45;
	};

}

namespace meta_scene::objects {

	struct camera final {
		matrix4x4 transform = matrix4x4(1);

		internal::depth_of_field depth_of_field;
		internal::perspective perspective;
		
		std::string type = "unknown";
	};
	
}
