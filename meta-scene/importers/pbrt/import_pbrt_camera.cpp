#include "import_pbrt_camera.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_film(scene_context& context, meta_scene::objects::film& film)
	{
		// the first token should be the name of film, but we do not need it
		// so we just peek it without processing
		context.peek_one_token();

		film.filter.type = "box";
		film.scale = 1;

		context.loop_important_token([&]
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_INTEGER_TOKEN) {
					const auto value = context.peek_integer<uint32>();

					if (name == "xresolution") META_SCENE_FINISHED_AND_RETURN(film.size_x = value);
					if (name == "yresolution") META_SCENE_FINISHED_AND_RETURN(film.size_y = value);
				}

				// because we do not process the "string filename" so we just remove the value 
				if (type == PBRT_STRING_TOKEN) META_SCENE_FINISHED_AND_RETURN(context.peek_one_token());

				// scale property 
				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();

					if (name == "scale") META_SCENE_FINISHED_AND_RETURN(film.scale = value);
					if (name == "maxsampleluminance") return;
				}
				
				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}

	void import_sampler(scene_context& context, meta_scene::objects::sampler& sampler)
	{
		sampler.type = "random";
		
		// the first token should be the type of sampler
		// but we only support random sampler now
		const auto sampler_type = remove_special_character(context.peek_one_token());

		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_INTEGER_TOKEN) {
					const auto value = context.peek_integer<uint32>();
					
					if (name == "pixelsamples") META_SCENE_FINISHED_AND_RETURN(sampler.sample_per_pixel = value);
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}

	void import_box_filter(scene_context& context, meta_scene::objects::filter& filter)
	{
		filter.type = "box";
		filter.radius_x = 0.5;
		filter.radius_y = 0.5;

		context.loop_important_token([&]()
			{
			});
	}

	void import_gaussian_filter(scene_context& context, meta_scene::objects::filter& filter)
	{
		filter.type = "gaussian";
		filter.radius_x = 2;
		filter.radius_y = 2;
		filter.gaussian.alpha = 2;

		context.loop_important_token([&]()
			{
			});
	}
	
	void import_filter(scene_context& context, meta_scene::objects::filter& filter)
	{
		const auto name = remove_special_character(context.peek_one_token());

		if (name == "gaussian") import_gaussian_filter(context, filter);
		if (name == "box") import_box_filter(context, filter);

		META_SCENE_IMPORT_SUCCESS_CHECK_TYPE(filter);
	}

	void import_perspective_camera(scene_context& context, meta_scene::objects::camera& camera)
	{
		camera.type = "perspective";
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();
					
					if (name == "fov") META_SCENE_FINISHED_AND_RETURN(camera.perspective.fov = value);
					if (name == "focaldistance") META_SCENE_FINISHED_AND_RETURN(camera.depth_of_field.focus = value);
					if (name == "lensradius") META_SCENE_FINISHED_AND_RETURN(camera.depth_of_field.lens = value);
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}

	void import_camera(scene_context& context, meta_scene::objects::camera& camera, const matrix4x4& transform)
	{
		const auto type = remove_special_character(context.peek_one_token());
		
		if (type == "perspective") import_perspective_camera(context, camera);

		camera.transform = transform;

		context.state.transforms.insert({ "camera", transform });
		
		META_SCENE_IMPORT_SUCCESS_CHECK_TYPE(camera);
	}
}

#endif