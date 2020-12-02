#pragma once

#include "import_pbrt_include.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_integrator(scene_context& context, meta_scene::objects::integrator& integrator);
	
}

#endif