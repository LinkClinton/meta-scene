#pragma once

#include "import_pbrt_include.hpp"

#include "../../spectrums/spectrum.hpp"

#ifdef __PBRT_IMPORTER__

using namespace metascene::spectrums;

namespace metascene::importers::pbrt {

	void import_color_spectrum(const std::string& token, std::shared_ptr<spectrum>& spectrum);
	
	void import_color_spectrum(scene_context& context, std::shared_ptr<spectrum>& spectrum);
	
}

#endif