#pragma once

#include "camera.hpp"

namespace metascene {

	namespace cameras {

		struct perspective_camera final : camera {
			real near = static_cast<real>(0.01);
			real far = static_cast<real>(1000.0);
			real fov = static_cast<real>(45.0);

			perspective_camera();
		};
		
	}
}
