#include "import_pbrt_medium.hpp"

#include "../../media/homogeneous_medium.hpp"
#include "../../logs.hpp"

#include "import_pbrt_spectrum.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	void import_homogeneous_medium(scene_context& context, const property_group& properties, std::shared_ptr<medium>& medium)
	{
		auto instance = std::make_shared<homogeneous_medium>();

		instance->sigma_a = std::make_shared<color_spectrum>(static_cast<real>(0.0011), static_cast<real>(0.0024), static_cast<real>(0.014));
		instance->sigma_s = std::make_shared<color_spectrum>(static_cast<real>(2.55), static_cast<real>(3.21), static_cast<real>(3.77));
		instance->g = 0;

		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "sigma_a") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum(value, instance->sigma_a));
				if (name == "sigma_s") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum(value, instance->sigma_s));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = string_to_real(remove_special_character(property.second));

				if (name == "g") META_SCENE_FINISHED_AND_CONTINUE(instance->g = value);
			}

			if (type == PBRT_STRING_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "type") continue;
			}
		}

		medium = instance;
	}

	void import_medium_from_property_group(scene_context& context, const property_group& properties, std::shared_ptr<medium>& medium)
	{
		const auto type = properties.find(type_and_name(PBRT_STRING_TOKEN, "type"))->second;

		std::shared_ptr<metascene::medium> instance = nullptr;

		if (type == "homogeneous") import_homogeneous_medium(context, properties, instance);
		
		META_SCENE_IMPORT_SUCCESS_CHECK(instance);

		medium = instance;
	}

	void import_named_medium(scene_context& context)
	{
		// the first token should be the name of material
		const auto name = remove_special_character(context.peek_one_token());

		property_group properties;

		context.loop_important_token([&]()
			{
				const auto type_and_name = context.peek_type_and_name();
				const auto value = context.peek_one_token();

				properties.insert({ type_and_name, value });
			});

		properties[type_and_name(PBRT_STRING_TOKEN, "type")] =
			read_string_from_token(properties[type_and_name(PBRT_STRING_TOKEN, "type")]);

		import_medium_from_property_group(context, properties, context.state.media[name]);
	}

	void import_named_medium(scene_context& context, std::shared_ptr<medium>& medium)
	{
		const auto name = read_string_from_token(context.peek_one_token());

		medium = context.state.find_medium(name);
	}

	void import_medium_interface(scene_context& context, std::shared_ptr<metascene::media::media>& media)
	{
		const auto interior = read_string_from_token(context.peek_one_token());
		const auto exterior = read_string_from_token(context.peek_one_token());

		media = std::make_shared<metascene::media::media>();
		
		media->inside = context.state.find_medium(interior);
		media->outside = context.state.find_medium(exterior);
	}
}

#endif