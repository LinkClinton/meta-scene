#include "import_pbrt_spectrum.hpp"

#include "../../spectrums/sampled_spectrum.hpp"
#include "../../spectrums/color_spectrum.hpp"

#ifdef __PBRT_IMPORTER__

using namespace metascene::spectrums;

namespace metascene::importers::pbrt {

	void import_color_spectrum(const std::string& token, std::shared_ptr<spectrum>& spectrum)
	{
		auto instance = std::make_shared<color_spectrum>();

		auto stream = std::stringstream(remove_special_character(token));

		stream >> instance->red >> instance->green >> instance->blue;

		spectrum = instance;
	}

	void import_sampled_spectrum(const std::string& filename, std::shared_ptr<spectrum>& spectrum)
	{
		spectrum = read_sampled_spectrum_from_spd(filename);
	}

	void import_black_body_spectrum(const std::string& token, std::shared_ptr<spectrums::spectrum>& spectrum)
	{
		auto stream = std::stringstream(remove_special_character(token));

		real temperature, scale;

		stream >> temperature >> scale;

		spectrum = create_sampled_spectrum_from_black_body(temperature, scale);
	}
}

#endif