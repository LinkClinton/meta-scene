#include "import_pbrt_light_source.hpp"
#include "import_pbrt_spectrum.hpp"

#include "../../emitters/environment_emitter.hpp"
#include "../../emitters/surface_emitter.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	void import_infinite_light(scene_context& context, std::shared_ptr<emitter>& emitter)
	{
		auto instance = std::make_shared<environment_emitter>();
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_STRING_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "mapname") META_SCENE_FINISHED_AND_RETURN(instance->environment_map = context.directory_path + value);
				}

				if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
					const auto value = context.peek_one_token();
					
					if (name == "scale") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum(value, instance->intensity));
					if (name == "L") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum(value, instance->intensity));
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		emitter = instance;
	}

	void import_area_light(scene_context& context, std::shared_ptr<emitter>& emitter)
	{
		auto instance = std::make_shared<surface_emitter>();

		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
					const auto value = context.peek_one_token();

					if (name == "scale") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum(value, instance->radiance));
					if (name == "L") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum(value, instance->radiance));
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		emitter = instance;
	}

	void import_light_source(scene_context& context)
	{
		// the first token should be the type of light source
		const auto type = remove_special_character(context.peek_one_token());

		const auto entity = std::make_shared<metascene::entity>();
		
		if (type == "infinite") import_infinite_light(context, entity->emitter);

		entity->transform = context.current().transform;

		context.scene->entities.push_back(entity);

		META_SCENE_IMPORT_SUCCESS_CHECK(entity->emitter);
	}

	void import_area_light_source(scene_context& context, std::shared_ptr<emitter>& emitter)
	{
		const auto type = remove_special_character(context.peek_one_token());

		if (type == "diffuse") import_area_light(context, emitter);

		META_SCENE_IMPORT_SUCCESS_CHECK(emitter);
	}
}

#endif