#pragma once

#include "../filters/filter.hpp"

#include <memory>

namespace metascene {

	using namespace filters;
	
	namespace cameras {

		struct film {
			std::shared_ptr<filter> filter;
			
			size_t height = 720;
			size_t width = 1280;
			
			film() = default;
		};
		
	}
}
