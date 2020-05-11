#pragma once

#include "materials/material.hpp"
#include "emitters/emitter.hpp"
#include "math/matrix4x4.hpp"
#include "shapes/shape.hpp"

#include <memory>

namespace metascene {

	using namespace materials;
	using namespace emitters;
	using namespace shapes;
	using namespace math;
	
	struct entity final {
		std::shared_ptr<material> material;
		std::shared_ptr<emitter> emitter;
		std::shared_ptr<shape> shape;

		matrix4x4 transform;

		entity() = default;
	};
	
}
