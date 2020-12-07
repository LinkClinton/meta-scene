#pragma once

#include "../types.hpp"

namespace meta_scene::objects::internal {

	struct directional_light final {
		vector3 direction = vector3(0);
	};

	struct environment_light final {
		std::string image = "";

		bool gamma = false;
	};
	
}

namespace meta_scene::objects {

	struct light final {
		spectrum intensity = spectrum(0);

		internal::directional_light directional;
		internal::environment_light environment;
		
		std::string type = "unknown";
	};
	
}
