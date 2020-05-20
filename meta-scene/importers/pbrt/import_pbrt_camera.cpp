#include "import_pbrt_camera.hpp"

#include "../../cameras/perspective_camera.hpp"
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
					const auto value = context.peek_integer<size_t>();

					if (name == "xresolution") film->width = value;
					if (name == "yresolution") film->height = value;
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
					const auto value = context.peek_integer<size_t>();
					
					if (name == "pixelsamples") sampler->sample_per_pixel = value;
				}
			});
	}

	void import_perspective_camera(scene_context& context, std::shared_ptr<camera>& camera)
	{
		camera = std::make_shared<perspective_camera>();

		std::static_pointer_cast<perspective_camera>(camera)->system = coordinate_system::left_hand;
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();
					
					if (name == "fov") std::static_pointer_cast<perspective_camera>(camera)->fov = value;
				}
			});
	}

	void import_camera(scene_context& context, std::shared_ptr<camera>& camera)
	{
		const auto type = remove_special_character(context.peek_one_token());

		if (type == "perspective") import_perspective_camera(context, camera);
	}
}

#endif