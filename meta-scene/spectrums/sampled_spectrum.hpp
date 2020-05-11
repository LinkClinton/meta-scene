#pragma once

#include "spectrum.hpp"

#include <vector>

namespace metascene {

	namespace spectrums {

		struct sampled_spectrum final : spectrum {
			std::vector<real> lambda;
			std::vector<real> value;

			sampled_spectrum();
		};
		
	}
}
