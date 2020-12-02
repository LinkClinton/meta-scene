#pragma once

#include "import_pbrt_include.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_shape(scene_context& context, meta_scene::objects::entity& entity);

	void import_shape_to(scene_context& context);

	void import_objects_to(scene_context& context);
	
	void import_objects(scene_context& context);
}

#endif