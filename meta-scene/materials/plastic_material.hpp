#pragma once

#include "../spectrums/color_spectrum.hpp"
#include "../spectrums/spectrum.hpp"
#include "material.hpp"

#include <memory>

namespace metascene {

	using namespace spectrums;
	
	namespace materials {

		struct plastic_material final : material {
			std::shared_ptr<spectrum> specular = std::make_shared<color_spectrum>();
			std::shared_ptr<spectrum> diffuse = std::make_shared<color_spectrum>();

			bool remapped_roughness_to_alpha = false;
			
			real roughness = static_cast<real>(0.5);
			real eta = static_cast<real>(1.5);

			plastic_material();

			std::string to_string() const noexcept override;
		};
		
	}
}
