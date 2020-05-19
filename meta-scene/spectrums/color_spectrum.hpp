#pragma once

#include "spectrum.hpp"

namespace metascene {

	namespace spectrums {

		struct color_spectrum final : spectrum {
			real red = 1, green = 1, blue = 1;
			
			color_spectrum();

			color_spectrum(real red, real green, real blue);

			color_spectrum(real value);
			
			std::string to_string() const noexcept override;
		};
		
	}
}
