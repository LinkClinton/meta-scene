#pragma once

#include "../textures/constant_texture.hpp"
#include "material.hpp"

namespace metascene {

	using namespace textures;
	
	namespace materials {

		struct metal_material final : material {
			std::shared_ptr<texture> eta = std::make_shared<constant_texture>();
			std::shared_ptr<texture> k = std::make_shared<constant_texture>();
			
			std::shared_ptr<texture> roughness_u = std::make_shared<constant_texture>(static_cast<real>(0));
			std::shared_ptr<texture> roughness_v = std::make_shared<constant_texture>(static_cast<real>(0));
			
			bool remapped_roughness_to_alpha = true;

			metal_material();

			std::string to_string() const noexcept override;
		};
		
	}
}
