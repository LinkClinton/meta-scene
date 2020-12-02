#pragma once

#include "texture.hpp"

#include <array>

namespace meta_scene::objects::internal {

	struct curve final {
		std::array<vector3, 4> control_points = {};
		std::array<real, 2> width = { 1, 1 };

		real u_min = 0;
		real u_max = 0;
	};

	struct disk final {
		real radius = 1;
		real height = 0;
	};

	struct mesh final {
		std::string type = "unknown";
		std::string filename = "";
		
		std::optional<texture> mask;
	};

	struct sphere final {
		real radius = 1;
	};

	struct triangles final {
		std::vector<vector3> positions;
		std::vector<vector3> normals;
		std::vector<vector3> uvs;

		std::vector<uint32> indices;

		std::optional<texture> mask;
	};
}

namespace meta_scene::objects {

	struct shape final {
		bool reverse_orientation = false;

		internal::triangles triangles;
		internal::sphere sphere;
		internal::curve curve;
		internal::mesh mesh;
		internal::disk disk;
		
		std::string type = "unknown";
	};
	
}
