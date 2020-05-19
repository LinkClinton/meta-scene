#pragma once

#include "../textures/constant_texture.hpp"
#include "material.hpp"

#include <memory>

namespace metascene {

	using namespace textures;
	
	namespace materials {

		struct diffuse_material final : material {
			std::shared_ptr<texture> reflectance;
			std::shared_ptr<texture> sigma;
			
			diffuse_material();

			std::string to_string() const noexcept override;
		};
		
	}
}
