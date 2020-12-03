#pragma once

#include "texture.hpp"

namespace meta_scene::objects {
	struct material;
}

namespace meta_scene::objects::internal {

	struct material_mixture_component final {
		std::shared_ptr<material> material0;
		std::shared_ptr<material> material1;

		texture alpha;
	};

}

namespace meta_scene::objects {

	struct material final {
		internal::material_mixture_component mixture;
		
		mapping<std::string, texture> properties;
		
		bool remapping = true;
		
		std::string type = "unknown";
	};
	
}
