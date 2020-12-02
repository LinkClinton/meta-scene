#pragma once

#include "import_pbrt_include.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_medium_from_property_group(scene_context& context, const property_group& properties, 
		std::optional<meta_scene::objects::medium>& medium);

	void import_named_medium(scene_context& context);

	void import_named_medium(scene_context& context, std::optional<meta_scene::objects::medium>& medium);

	void import_medium_interface(scene_context& context, std::optional<meta_scene::objects::media>& media);
}

#endif