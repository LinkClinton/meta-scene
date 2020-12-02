#pragma once

#include "import_pbrt_include.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_light_source(scene_context& context);

	void import_area_light_source(scene_context& context, std::optional<meta_scene::objects::light>& light);
}

#endif