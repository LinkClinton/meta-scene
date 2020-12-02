#include "import_pbrt_spectrum.hpp"

#include "../../shared/spectrum.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_color_spectrum(const std::string& token, spectrum& spectrum)
	{
		auto stream = std::stringstream(remove_special_character(token));

		stream >> spectrum.r >> spectrum.g >> spectrum.b;
	}

	void import_sampled_spectrum(const std::string& filename, spectrum& spectrum)
	{
		spectrum = shared::create_sampled_spectrum_from_spd(filename);
	}

	void import_black_body_spectrum(const std::string& token, spectrum& spectrum)
	{
		auto stream = std::stringstream(remove_special_character(token));

		real temperature, scale;

		stream >> temperature >> scale;

		spectrum = shared::create_sampled_spectrum_from_black_body(temperature, scale);
	}
	
}

#endif