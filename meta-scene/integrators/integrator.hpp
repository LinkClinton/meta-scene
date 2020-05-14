#pragma once

#include "../utilities.hpp"

#include <string>

namespace metascene {

	namespace integrators {

		enum class type : unsigned {
			unknown = 0, path = 1, direct = 2
		};
		
		struct integrator {
			type type = type::unknown;

			size_t depth = 5;

			integrator() = default;

			integrator(const integrators::type& type);
		};
	
	}
}
