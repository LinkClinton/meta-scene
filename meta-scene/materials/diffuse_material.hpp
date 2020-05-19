#pragma once

#include "../textures/constant_texture.hpp"
#include "material.hpp"

#include <memory>

namespace metascene {

	using namespace textures;
	
	namespace materials {

		struct diffuse_material final : material {
			std::shared_ptr<texture> reflectance = std::make_shared<constant_texture>();
			std::shared_ptr<texture> sigma = std::make_shared<constant_texture>(static_cast<real>(0));
			
			diffuse_material();

			std::string to_string() const noexcept override;
		};
		
	}
}
