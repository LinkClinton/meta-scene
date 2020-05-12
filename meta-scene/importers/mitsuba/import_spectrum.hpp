#pragma once

#include "../../spectrums/spectrum.hpp"

#include "import_include.hpp"

#ifdef __MITSUBA_IMPORTER__

using namespace metascene::spectrums;

namespace metascene::importers::mitsuba {

	void import_spectrum(const tinyxml2::XMLNode* node, std::shared_ptr<spectrum>& spectrum);

	void import_rgb(const tinyxml2::XMLNode* node, std::shared_ptr<spectrum>& spectrum);
}

#endif