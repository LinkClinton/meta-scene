#pragma once

#include "../spectrums/color_spectrum.hpp"
#include "texture.hpp"

namespace metascene {

	using namespace spectrums;
	
	namespace textures {

		struct constant_texture final : texture {
			std::shared_ptr<spectrum> spectrum = std::make_shared<color_spectrum>();

			real real = static_cast<metascene::real>(0);

			constant_texture();

			constant_texture(metascene::real value);

			std::string to_string() const noexcept override;
		};
		
	}
}
