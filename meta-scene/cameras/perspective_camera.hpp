#pragma once

#include "camera.hpp"

namespace metascene {

	namespace cameras {

		struct perspective_camera final : camera {
			real near = 0.01;
			real far = 1000.0;
			real fov = 45.0;

			perspective_camera();
		};
		
	}
}
