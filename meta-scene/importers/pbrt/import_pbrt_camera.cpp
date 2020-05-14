#include "import_pbrt_camera.hpp"

#include "../../samplers/random_sampler.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	void import_film(scene_context& context, std::shared_ptr<film>& film)
	{
		film = std::make_shared<metascene::film>();
		// the first token should be the name of film, but we do not need it
		// so we just peek it without processing
		context.peek_one_token();

		context.loop_important_token([&]
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_INTEGER_TOKEN) {
					const auto value_token = remove_special_character(context.peek_one_token());

					if (name == "xresolution") film->width = string_to_integer<size_t>(value_token);
					if (name == "yresolution") film->height = string_to_integer<size_t>(value_token);
				}

				// because we do not process the "string filename" so we just remove the value 
				if (type == PBRT_STRING_TOKEN) context.peek_one_token();
			});
	}

	void import_sampler(scene_context& context, std::shared_ptr<sampler>& sampler)
	{
		sampler = std::make_shared<random_sampler>();
		
		// the first token should be the type of sampler
		// but we only support random sampler now
		const auto sampler_type = remove_special_character(context.peek_one_token());

		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_INTEGER_TOKEN) {
					const auto value_token = remove_special_character(context.peek_one_token());

					if (name == "pixelsamples") sampler->sample_per_pixel = string_to_integer<size_t>(value_token);
				}
			});
	}

	void import_look_at(scene_context& context, matrix4x4& transform)
	{
		
	}
}

#endif