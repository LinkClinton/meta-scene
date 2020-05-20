#pragma once

#include "camera.hpp"

namespace metascene {

	namespace cameras {

		enum class coordinate_system : uint32 {
			right_hand = 0, left_hand = 1
		};
		
		struct perspective_camera final : camera {
			coordinate_system system = coordinate_system::right_hand;
			
			real near = static_cast<real>(0.01);
			real far = static_cast<real>(1000.0);
			real fov = static_cast<real>(45.0);

			perspective_camera();
		};
		
	}
}
