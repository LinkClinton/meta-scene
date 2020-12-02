#pragma once

#include "../types.hpp"

#include <memory>

namespace meta_scene::objects {
	struct texture;
}

namespace meta_scene::objects::internal {

	struct constant_texture final {
		spectrum value = spectrum(0);
	};

	struct image_texture final {
		std::string filename = "";

		bool gamma = false;
	};

	struct scale_texture final {
		std::shared_ptr<texture> scale;
		std::shared_ptr<texture> base;
	};

	struct mixture_texture final {
		std::shared_ptr<texture> texture0;
		std::shared_ptr<texture> texture1;
		std::shared_ptr<texture> alpha;
	};
	
}

namespace meta_scene::objects {

	struct texture final {
		internal::constant_texture constant;
		internal::mixture_texture mixture;
		internal::image_texture image;
		internal::scale_texture scale;
		
		std::string type = "unknown";
	};
	
}
