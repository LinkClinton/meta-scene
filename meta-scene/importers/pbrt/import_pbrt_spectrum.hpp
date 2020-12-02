#pragma once

#include "import_pbrt_include.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_color_spectrum(const std::string& token, spectrum& spectrum);
	
	void import_sampled_spectrum(const std::string& filename, spectrum& spectrum);

	void import_black_body_spectrum(const std::string& token, spectrum& spectrum);
	
}

#endif