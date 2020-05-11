#pragma once

#include "spectrum.hpp"

namespace metascene {

	namespace spectrums {

		struct color_spectrum final : spectrum {
			real red = 1, green = 1, blue = 1;
			
			color_spectrum();
		};
		
	}
}
