#pragma once

#include "filter.hpp"

namespace meta_scene::objects {

	struct film final {
		uint32 size_x = 0;
		uint32 size_y = 0;
		real scale = 1;

		filter filter;
	};
	
}
