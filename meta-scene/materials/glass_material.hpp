#pragma once

#include "../spectrums/color_spectrum.hpp"
#include "../spectrums/spectrum.hpp"
#include "material.hpp"

namespace metascene {

	using namespace spectrums;
	
	namespace materials {

		struct glass_material final : material {
			std::shared_ptr<spectrum> transmission = std::make_shared<color_spectrum>();
			std::shared_ptr<spectrum> reflectance = std::make_shared<color_spectrum>();

			real roughness_u = static_cast<real>(0);
			real roughness_v = static_cast<real>(0);
			real eta = static_cast<real>(1.5);

			bool remapped_roughness_to_alpha = false;

			glass_material();

			std::string to_string() const noexcept override;
		};
		
	}
}
