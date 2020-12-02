#pragma once

#include "../types.hpp"

namespace meta_scene::objects::internal {

	struct direct_integrator final {
		uint32 light_samples = 4;
		uint32 bsdf_samples = 4;
	};

	struct path_integrator final {
		real threshold = 1;
	};

	struct photon_mapping_integrator final {
		uint32 iterations = 64;
		uint32 photons = 0;

		real radius = 1;
	};
	
}

namespace meta_scene::objects {

	struct integrator final {
		uint32 depth = 5;

		internal::photon_mapping_integrator photon_mapping;
		internal::direct_integrator direct;
		internal::path_integrator path;
		
		std::string type = "unknown";
	};
	
}
