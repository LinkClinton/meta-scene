#pragma once

#include "import_pbrt_include.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_color_spectrum_texture(const std::string& token, meta_scene::objects::texture& texture);

	void import_sampled_spectrum_texture(const std::string& filename, meta_scene::objects::texture& texture);
	
	void import_real_texture(const std::string& token, meta_scene::objects::texture& texture);

	void import_real_texture(const std::string& token, meta_scene::objects::texture& texture, uint32 channel);
	
	void import_texture(scene_context& context);
}

#endif