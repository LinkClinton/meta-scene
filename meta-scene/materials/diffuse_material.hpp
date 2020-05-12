#pragma once

#include "../spectrums/color_spectrum.hpp"
#include "../spectrums/spectrum.hpp"
#include "material.hpp"

#include <memory>

namespace metascene {

	using namespace spectrums;

	namespace materials {

		struct diffuse_material final : material {
			std::shared_ptr<spectrum> reflectance = std::make_shared<color_spectrum>();

			diffuse_material();

			std::string to_string() const noexcept override;
		};
		
	}
}
