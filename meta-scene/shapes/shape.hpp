#pragma once

#include "../utilities.hpp"

namespace metascene {

	namespace shapes {

		enum class type : uint32 {
			unknown = 0, mesh = 1
		};

		struct shape {
			bool reverse_orientation = false;

			type type = type::unknown;

			shape() = default;

			shape(const shapes::type& type);
		};
	}
}
