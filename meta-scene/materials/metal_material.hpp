#pragma once

#include "../spectrums/color_spectrum.hpp"
#include "../spectrums/spectrum.hpp"
#include "material.hpp"

namespace metascene {

	using namespace spectrums;
	
	namespace materials {

		struct metal_material final : material {
			std::shared_ptr<spectrum> eta;
			std::shared_ptr<spectrum> k;

			real roughness_u = static_cast<real>(0);
			real roughness_v = static_cast<real>(0);

			bool remapped_roughness_to_alpha = true;

			metal_material();

			std::string to_string() const noexcept override;
		};
		
	}
}
