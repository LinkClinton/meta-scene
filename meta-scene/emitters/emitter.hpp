#pragma once

#include "../utilities.hpp"

namespace metascene {

	namespace emitters {

		enum class type : uint32 {
			unknown = 0, surface = 1
		};
		
		struct emitter {
			type type = type::unknown;

			emitter() = default;

			emitter(const emitters::type& type);
		};
		
	}
}
