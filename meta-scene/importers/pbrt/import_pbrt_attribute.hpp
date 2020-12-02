#pragma once

#include "import_pbrt_include.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_attribute(scene_context& context);
	
}

#endif