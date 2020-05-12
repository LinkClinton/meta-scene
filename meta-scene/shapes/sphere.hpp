#pragma once

#include "../math/vector3.hpp"

#include "shape.hpp"

namespace metascene {

	using namespace math;
	
	namespace shapes {

		struct sphere final : shape {
			vector3 center = vector3(0);
			real radius = static_cast<real>(1);

			sphere();
		};
		
	}
}
