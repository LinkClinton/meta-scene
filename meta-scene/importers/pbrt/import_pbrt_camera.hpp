#pragma once

#include "import_pbrt_include.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_film(scene_context& context, meta_scene::objects::film& film);

	void import_sampler(scene_context& context, meta_scene::objects::sampler& sampler);

	void import_filter(scene_context& context, meta_scene::objects::filter& filter);
	
	void import_camera(scene_context& context, meta_scene::objects::camera& camera, const matrix4x4& transform);
}

#endif