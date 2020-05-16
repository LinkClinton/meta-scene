#pragma once

#include "../math/vector3.hpp"
#include "shape.hpp"

#include <vector>

namespace metascene {

	using namespace math;
	
	namespace shapes {

		struct triangles final : shape {
			std::vector<unsigned> indices;

			std::vector<vector3> positions;
			std::vector<vector3> normals;
			std::vector<vector3> uvs;

			triangles();

			std::string to_string() const noexcept override;
		};
	}
}
