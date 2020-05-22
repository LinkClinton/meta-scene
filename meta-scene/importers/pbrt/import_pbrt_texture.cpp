#include "import_pbrt_texture.hpp"

#include "import_pbrt_spectrum.hpp"

#include "../../textures/constant_texture.hpp"
#include "../../textures/image_texture.hpp"
#include "../../textures/scale_texture.hpp"
#include "../../textures/texture.hpp"

#include <filesystem>

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	void import_color_spectrum_texture(const std::string& token, std::shared_ptr<texture>& texture)
	{
		auto instance = std::make_shared<constant_texture>(value_type::spectrum);

		import_color_spectrum(token, instance->spectrum);

		texture = instance;
	}

	void import_sampled_spectrum_texture(const std::string& filename, std::shared_ptr<texture>& texture)
	{
		auto instance = std::make_shared<constant_texture>(value_type::spectrum);

		import_sampled_spectrum(filename, instance->spectrum);

		texture = instance;
	}

	void import_real_texture(const std::string& token, std::shared_ptr<texture>& texture)
	{
		const auto instance = std::make_shared<constant_texture>(value_type::real);

		instance->real = string_to_real(token);

		texture = instance;
	}

	void import_image_map_texture(scene_context& context, std::shared_ptr<texture>& texture)
	{
		auto instance = std::make_shared<image_texture>();
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_STRING_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "filename") META_SCENE_FINISHED_AND_RETURN(instance->filename = context.directory_path + value);
				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();

					if (name == "uscale") return;
					if (name == "vscale") return;
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		const auto extension = std::filesystem::path(instance->filename).extension().string();

		if (extension != ".hdr" && extension != ".exr") instance->gamma = true;
		
		texture = instance;
	}

	void import_scale_texture(scene_context& context, std::shared_ptr<texture>& texture)
	{
		auto instance = std::make_shared<scale_texture>();

		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_TEXTURE_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "tex1") META_SCENE_FINISHED_AND_RETURN(instance->base = context.state.textures[value]);
					if (name == "tex2") META_SCENE_FINISHED_AND_RETURN(instance->scale = context.state.textures[value]);
				}

				if (type == PBRT_RGB_TOKEN) {
					const auto value = context.peek_one_token();

					if (name == "tex1") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum_texture(value, instance->base));
					if (name == "tex2") META_SCENE_FINISHED_AND_RETURN(import_color_spectrum_texture(value, instance->scale));
				}

				if (type == PBRT_FLOAT_TOKEN) META_SCENE_FINISHED_AND_RETURN(context.peek_one_token());

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		if (instance->scale->type != textures::type::constant)
			META_SCENE_PBRT_SCALE_TEXTURE_SHOULD_CONSTANT;
		
		texture = instance;
	}

	void import_constant_texture(scene_context& context, std::shared_ptr<texture>& texture)
	{
		auto instance = std::make_shared<constant_texture>();

		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();

					if (name == "value") META_SCENE_FINISHED_AND_RETURN({ instance->value_type = value_type::real; instance->real = value; });
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		texture = instance;
	}
	
	void import_texture(scene_context& context)
	{
		const auto name = remove_special_character(context.peek_one_token());
		const auto color = remove_special_character(context.peek_one_token());
		const auto type = remove_special_character(context.peek_one_token());

		std::shared_ptr<texture> instance = nullptr;
		
		if (type == "constant") import_constant_texture(context, instance);
		if (type == "imagemap") import_image_map_texture(context, instance);
		if (type == "scale") import_scale_texture(context, instance);

		META_SCENE_IMPORT_SUCCESS_CHECK(instance);

		context.state.textures[name] = instance;
	}

}

#endif