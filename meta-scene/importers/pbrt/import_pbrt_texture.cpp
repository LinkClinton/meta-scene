#include "import_pbrt_texture.hpp"

#include "import_pbrt_spectrum.hpp"

#include <filesystem>

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_color_spectrum_texture(const std::string& token, meta_scene::objects::texture& texture)
	{
		import_color_spectrum(token, texture.constant.value);

		texture.type = "constant";
	}

	void import_sampled_spectrum_texture(const std::string& filename, meta_scene::objects::texture& texture)
	{
		import_sampled_spectrum(filename, texture.constant.value);

		texture.type = "constant";
	}

	void import_real_texture(const std::string& token, meta_scene::objects::texture& texture)
	{
		texture.constant.value = vector3(string_to_real(token));
		texture.type = "constant";
	}

	void import_image_map_texture(scene_context& context, meta_scene::objects::texture& texture)
	{
		texture.type = "image";
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_STRING_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "filename") META_SCENE_FINISHED_AND_RETURN(texture.image.filename = context.directory_path + value);
					if (name == "mapping") META_SCENE_FINISHED_AND_RETURN(warn("pbrt importer : mapping is not support."));
					if (name == "wrap") META_SCENE_FINISHED_AND_RETURN(warn("pbrt importer : wrap is not support."));

				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();

					if (name == "scale") META_SCENE_FINISHED_AND_RETURN(warn("pbrt importer : scale image map value is not support."))
					
					if (name == "uscale") META_SCENE_FINISHED_AND_RETURN(warn("pbrt importer : uscale is not support."));
					if (name == "vscale") META_SCENE_FINISHED_AND_RETURN(warn("pbrt importer : uscale is not support."));

					if (name == "udelta") META_SCENE_FINISHED_AND_RETURN(warn("pbrt importer : udelta is not support."));
					if (name == "vdelta") META_SCENE_FINISHED_AND_RETURN(warn("pbrt importer : udelta is not support."));

					if (name == "maxanisotropy") META_SCENE_FINISHED_AND_RETURN(warn("pbrt importer : maxanisotropy is not support"));
				}

				if (type == PBRT_BOOL_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "gamma") META_SCENE_FINISHED_AND_RETURN(texture.image.gamma = string_to_bool(value));
				}
			
				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		const auto extension = std::filesystem::path(texture.image.filename).extension().string();

		if (extension != ".hdr" && extension != ".exr") texture.image.gamma = true;
	}

	void import_scale_texture(scene_context& context, meta_scene::objects::texture& texture)
	{
		texture.type = "scale";
		
		texture.scale.scale = std::make_shared<meta_scene::objects::texture>();
		texture.scale.base = std::make_shared<meta_scene::objects::texture>();
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_TEXTURE_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "tex1") META_SCENE_FINISHED_AND_RETURN(*texture.scale.base = context.state.find_texture(value));
					if (name == "tex2") META_SCENE_FINISHED_AND_RETURN(*texture.scale.scale = context.state.find_texture(value));
				}

				if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
					const auto value = context.peek_one_token();

					if (name == "tex1") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum_texture(value, *texture.scale.base));
					if (name == "tex2") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum_texture(value, *texture.scale.scale));
				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_one_token();

					if (name == "tex2") META_SCENE_FINISHED_AND_RETURN(import_real_texture(value, *texture.scale.scale));
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		if (texture.scale.scale->type != "constant")
			META_SCENE_PBRT_SCALE_TEXTURE_SHOULD_CONSTANT;
	}

	void import_constant_texture(scene_context& context, meta_scene::objects::texture& texture)
	{
		texture.type = "constant";
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();

					if (name == "value") META_SCENE_FINISHED_AND_RETURN(texture.constant.value = spectrum(value));
				}

				if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
					const auto value = context.peek_one_token();

					if (name == "value") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum(value, texture.constant.value));
				}
			
				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}

	void import_marble_texture(scene_context& context, meta_scene::objects::texture& texture)
	{
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();

					if (name == "scale") return;
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
		
		warn("pbrt importer : marble texture is not supported. we will create default constant texture.");

		texture.type = "constant";
		texture.constant.value = spectrum(1);
	}

	void import_fbm_texture(scene_context& context, meta_scene::objects::texture& texture)
	{
		context.loop_important_token([&]()
			{
				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		warn("pbrt importer : fbm texture is not supported. we will create default constant texture.");

		texture.type = "constant";
		texture.constant.value = spectrum(1);
	}

	void import_wrinkled_texture(scene_context& context, meta_scene::objects::texture& texture)
	{
		context.loop_important_token([&]()
			{
				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		warn("pbrt importer : wrinkled texture is not supported. we will create default constant texture.");

		texture.type = "constant";
		texture.constant.value = spectrum(1);
	}

	void import_mixture_texture(scene_context& context, meta_scene::objects::texture& texture)
	{
		texture.type = "mixture";
		
		texture.mixture.texture0 = std::make_shared<meta_scene::objects::texture>();
		texture.mixture.texture1 = std::make_shared<meta_scene::objects::texture>();
		texture.mixture.alpha = std::make_shared<meta_scene::objects::texture>();

		texture.mixture.texture0->type = "constant";
		texture.mixture.texture1->type = "constant";
		texture.mixture.alpha->type = "constant";

		texture.mixture.texture0->constant.value = spectrum(0);
		texture.mixture.texture1->constant.value = spectrum(1);
		texture.mixture.alpha->constant.value = spectrum(0.5);
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();
			
				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_one_token();

					if (name == "tex1") META_SCENE_FINISHED_AND_RETURN(import_real_texture(value, *texture.mixture.texture0));
					if (name == "tex2") META_SCENE_FINISHED_AND_RETURN(import_real_texture(value, *texture.mixture.texture1));
					if (name == "amount") META_SCENE_FINISHED_AND_RETURN(import_real_texture(value, *texture.mixture.alpha));
				}

				if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
					const auto value = context.peek_one_token();

					if (name == "tex1") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum_texture(value, *texture.mixture.texture0));
					if (name == "tex2") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum_texture(value, *texture.mixture.texture1));
				}

				if (type == PBRT_TEXTURE_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "tex1") META_SCENE_FINISHED_AND_RETURN(*texture.mixture.texture0 = context.state.find_texture(value));
					if (name == "tex2") META_SCENE_FINISHED_AND_RETURN(*texture.mixture.texture1 = context.state.find_texture(value));
					if (name == "amount") META_SCENE_FINISHED_AND_RETURN(*texture.mixture.alpha = context.state.find_texture(value));
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}

	void import_windy_texture(scene_context& context, meta_scene::objects::texture& texture)
	{
		context.loop_important_token([&]()
			{
				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		warn("pbrt importer : windy texture is not supported. we will create default constant texture.");

		texture.type = "constant";
		texture.constant.value = spectrum(1);
	}

	void import_texture(scene_context& context)
	{
		const auto name = remove_special_character(context.peek_one_token());
		const auto color = remove_special_character(context.peek_one_token());
		const auto type = remove_special_character(context.peek_one_token());

		meta_scene::objects::texture texture;
		
		if (type == "constant") import_constant_texture(context, texture);
		if (type == "wrinkled") import_wrinkled_texture(context, texture);
		if (type == "imagemap") import_image_map_texture(context, texture);
		if (type == "marble") import_marble_texture(context, texture);
		if (type == "scale") import_scale_texture(context, texture);
		if (type == "windy") import_windy_texture(context, texture);
		if (type == "mix") import_mixture_texture(context, texture);
		if (type == "fbm") import_fbm_texture(context, texture);
		
		META_SCENE_IMPORT_SUCCESS_CHECK_TYPE(texture);

		context.state.textures[name] = texture;
	}

}

#endif