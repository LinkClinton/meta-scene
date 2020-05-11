#pragma once

#include "../utilities.hpp"

namespace metascene {

	namespace spectrums {

		enum class type : uint32 {
			unknown = 0, color = 1, sampled = 2
		};
		
		struct spectrum {
			type type = type::unknown;

			spectrum() = default;

			spectrum(const spectrums::type& type);
		};
		
	}
}
