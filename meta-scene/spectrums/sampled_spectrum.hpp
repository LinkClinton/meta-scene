#pragma once

#include "spectrum.hpp"

#include <vector>

namespace metascene {

	namespace spectrums {

		struct sampled_spectrum final : spectrum {
			std::vector<real> lambda;
			std::vector<real> value;

			sampled_spectrum();

			std::string to_string() const noexcept override;
		};

		std::shared_ptr<sampled_spectrum> read_sampled_spectrum_from_spd(const std::string& filename);
	}
}
