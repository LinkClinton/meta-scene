#pragma once

#include "import_pbrt_include.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	void import_pbrt_material(scene_context& context, std::shared_ptr<material>& material);
	
}

#endif