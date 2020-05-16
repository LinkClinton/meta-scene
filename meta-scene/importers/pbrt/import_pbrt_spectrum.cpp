#include "import_pbrt_spectrum.hpp"

#include "../../spectrums/color_spectrum.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	void import_color_spectrum(const std::string& token, std::shared_ptr<spectrum>& spectrum)
	{
		auto instance = std::make_shared<color_spectrum>();

		auto stream = std::stringstream(remove_special_character(token));

		stream >> instance->red >> instance->green >> instance->blue;

		spectrum = instance;
	}

	void import_color_spectrum(scene_context& context, std::shared_ptr<spectrum>& spectrum)
	{
		import_color_spectrum(context.peek_one_token(), spectrum);
	}
}

#endif