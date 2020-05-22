#include "import_pbrt_material.hpp"

#include "import_pbrt_texture.hpp"

#include "../../materials/diffuse_material.hpp"
#include "../../materials/plastic_material.hpp"
#include "../../materials/mirror_material.hpp"
#include "../../materials/glass_material.hpp"
#include "../../materials/metal_material.hpp"
#include "../../materials/uber_material.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	void import_plastic_material(scene_context& context, const property_group& properties, std::shared_ptr<material>& material)
	{
		auto instance = std::make_shared<plastic_material>();

		// initialize pbrt plastic material default value
		instance->specular = std::make_shared<constant_texture>(std::make_shared<color_spectrum>(static_cast<real>(0.25)));
		instance->diffuse = std::make_shared<constant_texture>(std::make_shared<color_spectrum>(static_cast<real>(0.25)));
		instance->roughness = std::make_shared<constant_texture>(static_cast<real>(0.1));
		instance->eta = std::make_shared<constant_texture>(static_cast<real>(1.5));
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, instance->diffuse));
				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, instance->specular));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(instance->specular = context.state.textures[value]);
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(instance->diffuse = context.state.textures[value]);

				// now we do not support it.
				if (name == "bumpmap") continue;;
			}
			
			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, instance->roughness));
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}

		material = instance;
	}

	void import_glass_material(scene_context& context, const property_group& properties, std::shared_ptr<material>& material)
	{
		const auto instance = std::make_shared<glass_material>();

		// initialize pbrt glass material default value
		instance->transmission = std::make_shared<constant_texture>(std::make_shared<color_spectrum>(static_cast<real>(1)));
		instance->reflectance = std::make_shared<constant_texture>(std::make_shared<color_spectrum>(static_cast<real>(1)));
		instance->roughness_u = std::make_shared<constant_texture>(static_cast<real>(0));
		instance->roughness_v = std::make_shared<constant_texture>(static_cast<real>(0));
		instance->eta = std::make_shared<constant_texture>(static_cast<real>(1.5));
		instance->remapped_roughness_to_alpha = true;
		
		for (const auto& property : properties)
		{
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, instance->reflectance));
				if (name == "Kt") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, instance->transmission));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, instance->roughness_u));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, instance->roughness_v));
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}

		material = instance;
	}

	void import_metal_material(scene_context& context, const property_group& properties, std::shared_ptr<material>& material)
	{
		auto instance = std::make_shared<metal_material>();

		// initialize pbrt metal material default value
		instance->eta = std::make_shared<constant_texture>(std::make_shared<color_spectrum>(static_cast<real>(0)));
		instance->k = std::make_shared<constant_texture>(std::make_shared<color_spectrum>(static_cast<real>(0)));
		instance->roughness_u = std::make_shared<constant_texture>(static_cast<real>(0.01));
		instance->roughness_v = std::make_shared<constant_texture>(static_cast<real>(0.01));
		instance->remapped_roughness_to_alpha = true;
		
		for (const auto& property : properties)
		{
			auto [type, name] = property.first;

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, instance->roughness_u));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, instance->roughness_v));

				if (name == "roughness") {
					import_real_texture(value, instance->roughness_u);
					import_real_texture(value, instance->roughness_v);

					continue;
				}
			}

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "eta") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, instance->eta));
				if (name == "k") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, instance->k));
			}

			if (type == PBRT_SPECTRUM_TOKEN) {
				const auto value = read_string_from_token(property.second);
				
				if (name == "eta") META_SCENE_FINISHED_AND_CONTINUE(import_sampled_spectrum_texture(context.directory_path + value, instance->eta));
				if (name == "k") META_SCENE_FINISHED_AND_CONTINUE(import_sampled_spectrum_texture(context.directory_path + value, instance->k));
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}

		material = instance;
	}

	void import_matte_material(scene_context& context, const property_group& properties, std::shared_ptr<material>& material)
	{
		auto instance = std::make_shared<diffuse_material>();

		// initialize pbrt matte material default value
		instance->reflectance = std::make_shared<constant_texture>(std::make_shared<color_spectrum>(static_cast<real>(0.5)));
		instance->sigma = std::make_shared<constant_texture>(static_cast<real>(0));
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, instance->reflectance));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "sigma") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, instance->sigma));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(instance->reflectance = context.state.textures[value]);
				if (name == "sigma") META_SCENE_FINISHED_AND_CONTINUE(instance->sigma = context.state.textures[value]);
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}

		material = instance;
	}

	void import_uber_material(scene_context& context, const property_group& properties, std::shared_ptr<material>& material)
	{
		auto instance = std::make_shared<uber_material>();

		// initialize pbrt uber material default value
		instance->specular = std::make_shared<constant_texture>(std::make_shared<color_spectrum>(static_cast<real>(0.25)));
		instance->diffuse = std::make_shared<constant_texture>(std::make_shared<color_spectrum>(static_cast<real>(0.25)));

		instance->transmission = std::make_shared<constant_texture>(std::make_shared<color_spectrum>(static_cast<real>(0)));
		instance->reflectance = std::make_shared<constant_texture>(std::make_shared<color_spectrum>(static_cast<real>(0)));

		instance->opacity = std::make_shared<constant_texture>(std::make_shared<color_spectrum>(static_cast<real>(1)));
		
		instance->roughness_u = std::make_shared<constant_texture>(static_cast<real>(0.1));
		instance->roughness_v = std::make_shared<constant_texture>(static_cast<real>(0.1));
		instance->eta = std::make_shared<constant_texture>(static_cast<real>(1.5));

		instance->remapped_roughness_to_alpha = true;
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, instance->roughness_u));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, instance->roughness_v));
				
				if (name == "index") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, instance->eta));

				if (name == "roughness") {
					import_real_texture(value, instance->roughness_u);
					import_real_texture(value, instance->roughness_v);

					continue;
				}
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(instance->specular = context.state.textures[value]);
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(instance->diffuse = context.state.textures[value]);

				if (name == "Kt") META_SCENE_FINISHED_AND_CONTINUE(instance->transmission = context.state.textures[value]);
				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(instance->reflectance = context.state.textures[value]);

				if (name == "opacity") META_SCENE_FINISHED_AND_CONTINUE(instance->opacity = context.state.textures[value]);
			}

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, instance->specular));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, instance->diffuse));

				if (name == "Kt") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, instance->transmission));
				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, instance->reflectance));

				if (name == "opacity") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, instance->opacity));
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;
			
			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
		
		material = instance;
	}

	void import_mirror_material(scene_context& context, const property_group& properties, std::shared_ptr<material>& material)
	{
		auto instance = std::make_shared<mirror_material>();

		instance->reflectance = std::make_shared<constant_texture>(static_cast<real>(0.9));
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, instance->reflectance));
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;
			
			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}

		material = instance;
	}
	
	void import_material_from_property_group(scene_context& context, const property_group& properties, std::shared_ptr<material>& material)
	{
		const auto type = properties.find(type_and_name(PBRT_STRING_TOKEN, "type"))->second;

		std::shared_ptr<materials::material> instance = nullptr;
		
		if (type == "plastic") import_plastic_material(context, properties, instance);
		if (type == "mirror") import_mirror_material(context, properties, instance);
		if (type == "glass") import_glass_material(context, properties, instance);
		if (type == "metal") import_metal_material(context, properties, instance);
		if (type == "matte") import_matte_material(context, properties, instance);
		if (type == "uber") import_uber_material(context, properties, instance);

		META_SCENE_IMPORT_SUCCESS_CHECK(instance);

		material = instance;
	}

	void import_material(scene_context& context, std::shared_ptr<material>& material)
	{
		const auto type = remove_special_character(context.peek_one_token());

		property_group properties;

		properties.insert({ type_and_name(PBRT_STRING_TOKEN, "type"), type });

		context.loop_important_token([&]()
			{
				const auto type_and_name = context.peek_type_and_name();
				const auto value = context.peek_one_token();

				properties.insert({ type_and_name, value });
			});

		import_material_from_property_group(context, properties, material);
	}

	void import_named_material(scene_context& context)
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
		
		import_material_from_property_group(context, properties, context.state.materials[name]);
	}

	void import_named_material(scene_context& context, std::shared_ptr<material>& material)
	{
		const auto name = read_string_from_token(context.peek_one_token());

		material = context.state.materials[name];
	}
}

#endif