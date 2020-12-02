#pragma once

#include "../types.hpp"

namespace meta_scene::objects {

	struct sampler final {
		uint32 sample_per_pixel = 16;

		std::string type = "unknown";
	};
	
}
