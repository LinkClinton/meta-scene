#include "import_pbrt_light_source.hpp"
#include "import_pbrt_spectrum.hpp"

#include <filesystem>

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_infinite_light(scene_context& context, meta_scene::objects::light& light)
	{
		light.environment.image = "";
		light.environment.gamma = false;
		light.intensity = spectrum(1);
		light.type = "environment";

		spectrum intensity = spectrum(1);
		spectrum scale = spectrum(1);
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_STRING_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "mapname") META_SCENE_FINISHED_AND_RETURN(light.environment.image = context.directory_path + value);
				}

				if (type == PBRT_INTEGER_TOKEN) {
					const auto value = context.peek_integer<size_t>();

					if (name == "nsamples") META_SCENE_FINISHED_AND_RETURN(warn("pbrt importer : samples is not support in infinte light."));
				}
			
				if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
					const auto value = context.peek_one_token();
					
					if (name == "scale") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum(value, scale));
					if (name == "L") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum(value, intensity));
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		const auto extension = std::filesystem::path(light.environment.image).extension().string();

		if (extension != ".hdr" && extension != ".exr") light.environment.gamma = true;

		light.intensity = intensity * scale;
	}

	void import_area_light(scene_context& context, meta_scene::objects::light& light)
	{
		light.type = "surface";

		spectrum intensity = spectrum(1);
		spectrum scale = spectrum(1);
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
					const auto value = context.peek_one_token();

					if (name == "scale") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum(value, scale));
					if (name == "L") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum(value, intensity));
				}

				if (type == PBRT_BLACK_BODY_TOKEN) {
					const auto value = context.peek_one_token();
					
					if (name == "L") META_SCENE_FINISHED_AND_RETURN(import_black_body_spectrum(value, intensity));
				}

				if (type == PBRT_INTEGER_TOKEN) {
					const auto value = context.peek_integer<size_t>();

					if (name == "nsamples") META_SCENE_FINISHED_AND_RETURN(warn("pbrt importer : samples is not support in area light."));
				}

				if (type == PBRT_SPECTRUM_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "L") META_SCENE_FINISHED_AND_RETURN(import_sampled_spectrum(context.directory_path + value, intensity));
				}
			
				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		light.intensity = intensity * scale;
	}

	void import_distant_light(scene_context& context, meta_scene::objects::light& light)
	{
		light.directional.from = vector3(0, 0, 0);
		light.directional.to = vector3(0, 0, 1);
		light.intensity = vector3(1);
		light.type = "directional";

		spectrum intensity = spectrum(1);
		spectrum scale = spectrum(1);
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
					const auto value = context.peek_one_token();

					if (name == "scale") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum(value, scale));
					if (name == "L") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum(value, intensity));
				}

				if (type == PBRT_POINT_TOKEN) {
					const auto value = context.peek_one_token();

					if (name == "from") META_SCENE_FINISHED_AND_RETURN(import_token_vector3(value, light.directional.from));
					if (name == "to") META_SCENE_FINISHED_AND_RETURN(import_token_vector3(value, light.directional.to));
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		light.intensity = intensity * scale;
	}

	void import_light_source(scene_context& context)
	{
		// the first token should be the type of light source
		const auto type = remove_special_character(context.peek_one_token());

		meta_scene::objects::entity entity;

		entity.light = meta_scene::objects::light();
		
		if (type == "infinite") import_infinite_light(context, entity.light.value());
		if (type == "distant") import_distant_light(context, entity.light.value());
		
		entity.transform = context.current().transform;

		context.scene.entities.push_back(entity);
		
		META_SCENE_IMPORT_SUCCESS_CHECK_TYPE(entity.light.value());
	}

	void import_area_light_source(scene_context& context, std::optional<meta_scene::objects::light>& light)
	{
		const auto type = remove_special_character(context.peek_one_token());

		light = meta_scene::objects::light();
		
		if (type == "diffuse") import_area_light(context, light.value());

		META_SCENE_IMPORT_SUCCESS_CHECK_TYPE(light.value());
	}
}

#endif