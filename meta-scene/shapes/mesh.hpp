#pragma once

#include "shape.hpp"

#include <string>

namespace metascene {

	namespace shapes {

		enum class mesh_type : uint32 {
			unknown = 0, obj = 1, ply = 2
		};
		
		struct mesh final : shape {
			mesh_type mesh_type = mesh_type::unknown;

			std::string filename = "unknown";
			
			mesh();

			mesh(const shapes::mesh_type& mesh_type, const std::string& filename);

			std::string to_string() const noexcept override;
		};
		
	}
}
