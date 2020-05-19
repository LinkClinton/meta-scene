#include "import_pbrt_texture.hpp"

#include "import_pbrt_spectrum.hpp"

#include "../../textures/constant_texture.hpp"
#include "../../textures/texture.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	void import_color_spectrum_texture(const std::string& token, std::shared_ptr<texture>& texture)
	{
		auto instance = std::make_shared<constant_texture>();

		import_color_spectrum(token, instance->spectrum);

		texture = instance;
	}

	void import_sampled_spectrum_texture(const std::string& filename, std::shared_ptr<texture>& texture)
	{
		auto instance = std::make_shared<constant_texture>();

		import_sampled_spectrum(filename, instance->spectrum);

		texture = instance;
	}

	void import_real_texture(const std::string& token, std::shared_ptr<texture>& texture)
	{
		auto instance = std::make_shared<constant_texture>();

		instance->real = string_to_real(token);

		texture = instance;
	}

}

#endif