#include "import_pbrt_medium.hpp"

#include "import_pbrt_spectrum.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_homogeneous_medium(scene_context& context, const property_group& properties, meta_scene::objects::medium& medium)
	{
		medium.type = "homogeneous";
		medium.homogeneous.sigma_a = spectrum(static_cast<real>(0.0011), static_cast<real>(0.0024), static_cast<real>(0.0140));
		medium.homogeneous.sigma_s = spectrum(static_cast<real>(2.5500), static_cast<real>(3.2100), static_cast<real>(3.7700));
		medium.homogeneous.g = 0;
		
		real scale = 1;
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "sigma_a") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum(value, medium.homogeneous.sigma_a));
				if (name == "sigma_s") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum(value, medium.homogeneous.sigma_s));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = string_to_real(remove_special_character(property.second));

				if (name == "g") META_SCENE_FINISHED_AND_CONTINUE(medium.homogeneous.g = value);
				if (name == "scale") META_SCENE_FINISHED_AND_CONTINUE(scale = value);
			}

			if (type == PBRT_STRING_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "type") continue;
			}
		}

		medium.homogeneous.sigma_a *= scale;
		medium.homogeneous.sigma_s *= scale;
	}

	void import_heterogeneous_medium(scene_context& context, const property_group& properties, meta_scene::objects::medium& medium)
	{
		medium.type = "heterogeneous";
		medium.heterogeneous.size_x = 1;
		medium.heterogeneous.size_y = 1;
		medium.heterogeneous.size_z = 1;
		medium.heterogeneous.g = 0;

		spectrum sigma_a = spectrum(static_cast<real>(0.0011), static_cast<real>(0.0024), static_cast<real>(0.0140));
		spectrum sigma_s = spectrum(static_cast<real>(2.5500), static_cast<real>(3.2100), static_cast<real>(3.7700));

		auto point0 = vector3(0);
		auto point1 = vector3(1);

		std::vector<real> density(1);

		real scale = 1;
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "sigma_a") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum(value, sigma_a));
				if (name == "sigma_s") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum(value, sigma_s));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "density") META_SCENE_FINISHED_AND_CONTINUE(import_token_real(value, density));
				if (name == "scale") META_SCENE_FINISHED_AND_CONTINUE(scale = string_to_real(value));
				if (name == "g") META_SCENE_FINISHED_AND_CONTINUE(medium.heterogeneous.g = string_to_real(value));
			}

			if (type == PBRT_POINT_TOKEN) {
				const auto value = property.second;

				if (name == "p0") META_SCENE_FINISHED_AND_CONTINUE(import_token_vector3(value, point0));
				if (name == "p1") META_SCENE_FINISHED_AND_CONTINUE(import_token_vector3(value, point1));
			}

			if (type == PBRT_INTEGER_TOKEN) {
				const auto value = string_to_integer<size_t>(remove_special_character(property.second));

				if (name == "nx") META_SCENE_FINISHED_AND_CONTINUE(medium.heterogeneous.size_x = value);
				if (name == "ny") META_SCENE_FINISHED_AND_CONTINUE(medium.heterogeneous.size_y = value);
				if (name == "nz") META_SCENE_FINISHED_AND_CONTINUE(medium.heterogeneous.size_z = value);
			}

			if (type == PBRT_STRING_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "type") continue;
			}

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}

		sigma_s = sigma_s * scale;
		sigma_a = sigma_a * scale;

		auto sigma_t = (sigma_s.r + sigma_a.r + sigma_s.g + sigma_a.g + sigma_s.b + sigma_a.b) / 3;
		
		medium.heterogeneous.albedo = sigma_s / sigma_t;
		medium.heterogeneous.sigma_t = std::vector<real>(density.begin(), 
			density.begin() + medium.heterogeneous.size_x * medium.heterogeneous.size_y * medium.heterogeneous.size_z);

		for (size_t index = 0; index < medium.heterogeneous.sigma_t.size(); index++)
			medium.heterogeneous.sigma_t[index] = medium.heterogeneous.sigma_t[index] * sigma_t;

		medium.heterogeneous.transform = context.current().transform * (
			glm::translate(matrix4x4(1), -point0) *
			glm::scale(matrix4x4(1), point1 - point0));
	}
	
	void import_medium_from_property_group(scene_context& context, const property_group& properties, 
		std::optional<meta_scene::objects::medium>& medium)
	{
		const auto type = properties.find(type_and_name(PBRT_STRING_TOKEN, "type"))->second;

		medium = meta_scene::objects::medium();
		
		if (type == "heterogeneous") import_heterogeneous_medium(context, properties, medium.value());
		if (type == "homogeneous") import_homogeneous_medium(context, properties, medium.value());
		
		META_SCENE_IMPORT_SUCCESS_CHECK_TYPE(medium.value());
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

	void import_named_medium(scene_context& context, std::optional<meta_scene::objects::medium>& medium)
	{
		const auto name = read_string_from_token(context.peek_one_token());

		medium = context.state.find_medium(name);
	}

	void import_medium_interface(scene_context& context, std::optional<meta_scene::objects::media>& media)
	{
		const auto interior = read_string_from_token(context.peek_one_token());
		const auto exterior = read_string_from_token(context.peek_one_token());

		media = meta_scene::objects::media();
		
		media->inside = context.state.find_medium(interior);
		media->outside = context.state.find_medium(exterior);
	}
}

#endif