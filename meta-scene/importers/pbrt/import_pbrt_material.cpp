#include "import_pbrt_material.hpp"

#include "import_pbrt_texture.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	constexpr auto META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT = "pbrt importer : bump map is not supported.";
	
	void import_plastic_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "plastic";
		material.properties["specular"].constant.value = spectrum(0.25f);
		material.properties["specular"].type = "constant";
		material.properties["diffuse"].constant.value = spectrum(0.25f);
		material.properties["diffuse"].type = "constant";
		material.properties["roughness"].constant.value = spectrum(0.1f);
		material.properties["roughness"].type = "constant";
		material.properties["eta"].constant.value = spectrum(1.5f);
		material.properties["eta"].type = "constant";
		material.remapping = true;
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["diffuse"]));
				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["specular"]));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(material.properties["specular"] = context.state.find_texture(value));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(material.properties["diffuse"] = context.state.find_texture(value));

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(material.properties["roughness"] = context.state.find_texture(value));
				
				// now we do not support it.
				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}
			
			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"]));
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_glass_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "glass";
		material.properties["transmission"].constant.value = spectrum(1);
		material.properties["transmission"].type = "constant";
		material.properties["reflectance"].constant.value = spectrum(1);
		material.properties["reflectance"].type = "constant";
		material.properties["roughness"].constant.value = spectrum(0);
		material.properties["roughness"].type = "constant";
		material.properties["eta"].constant.value = spectrum(1.5f);
		material.properties["eta"].type = "constant";
		material.remapping = true;
		
		for (const auto& property : properties)
		{
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["reflectance"]));
				if (name == "Kt") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["transmission"]));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"], 0));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"], 1));
				if (name == "index") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["eta"]));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);
				
				// now we do not support it.
				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_BOOL_TOKEN) {
				const auto value = string_to_bool(read_string_from_token(property.second));

				if (name == "remaproughness") META_SCENE_FINISHED_AND_CONTINUE(material.remapping = value);
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_conductor_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "conductor";
		material.properties["eta"].constant.value = spectrum(0);
		material.properties["eta"].type = "constant";
		material.properties["k"].constant.value = spectrum(0);
		material.properties["k"].type = "constant";
		material.properties["roughness"].constant.value = spectrum(0.01f);
		material.properties["roughness"].type = "constant";
		material.remapping = true;

		for (const auto& property : properties)
		{
			auto [type, name] = property.first;

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"], 0));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"], 1));

				if (name == "roughness")  META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"]));
			}

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "eta") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["eta"]));
				if (name == "k") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["k"]));
			}

			if (type == PBRT_SPECTRUM_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "eta") META_SCENE_FINISHED_AND_CONTINUE(import_sampled_spectrum_texture(context.directory_path + value, material.properties["eta"]));
				if (name == "k") META_SCENE_FINISHED_AND_CONTINUE(import_sampled_spectrum_texture(context.directory_path + value, material.properties["k"]));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(material.properties["roughness"] = context.state.find_texture(value));

				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_BOOL_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "remaproughness") META_SCENE_FINISHED_AND_CONTINUE(material.remapping = string_to_bool(value));
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_metal_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "metal";
		material.properties["eta"].constant.value = spectrum(0);
		material.properties["eta"].type = "constant";
		material.properties["k"].constant.value = spectrum(0);
		material.properties["k"].type = "constant";
		material.properties["roughness"].constant.value = spectrum(0.01f);
		material.properties["roughness"].type = "constant";
		material.remapping = true;
		
		for (const auto& property : properties)
		{
			auto [type, name] = property.first;

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"], 0));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"], 1));
				
				if (name == "roughness")  META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"]));
			}

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "eta") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["eta"]));
				if (name == "k") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["k"]));
			}

			if (type == PBRT_SPECTRUM_TOKEN) {
				const auto value = read_string_from_token(property.second);
				
				if (name == "eta") META_SCENE_FINISHED_AND_CONTINUE(import_sampled_spectrum_texture(context.directory_path + value, material.properties["eta"]));
				if (name == "k") META_SCENE_FINISHED_AND_CONTINUE(import_sampled_spectrum_texture(context.directory_path + value, material.properties["k"]));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(material.properties["roughness"] = context.state.find_texture(value));

				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_BOOL_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "remaproughness") META_SCENE_FINISHED_AND_CONTINUE(material.remapping = string_to_bool(value));
			}
			
			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_diffuse_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		// pbrt-v4 diffuse material
		material.type = "diffuse";

		material.properties["diffuse"].constant.value = spectrum(0.5f);
		material.properties["diffuse"].type = "constant";
		material.properties["sigma"].constant.value = spectrum(0);
		material.properties["sigma"].type = "constant";

		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kd" || name == "reflectance") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["diffuse"]));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "sigma") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["sigma"]));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Kd" || name == "reflectance") META_SCENE_FINISHED_AND_CONTINUE(material.properties["diffuse"] = context.state.find_texture(value));
				if (name == "sigma") META_SCENE_FINISHED_AND_CONTINUE(material.properties["sigma"] = context.state.find_texture(value));
				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_SPECTRUM_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Kd" || name == "reflectance") META_SCENE_FINISHED_AND_CONTINUE(import_sampled_spectrum_texture(context.directory_path + value, material.properties["diffuse"]));
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_matte_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "diffuse";

		material.properties["diffuse"].constant.value = spectrum(0.5f);
		material.properties["diffuse"].type = "constant";
		material.properties["sigma"].constant.value = spectrum(0);
		material.properties["sigma"].type = "constant";
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["diffuse"]));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "sigma") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["sigma"]));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(material.properties["diffuse"] = context.state.find_texture(value));
				if (name == "sigma") META_SCENE_FINISHED_AND_CONTINUE(material.properties["sigma"] = context.state.find_texture(value));
				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_SPECTRUM_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_sampled_spectrum_texture(context.directory_path + value, material.properties["diffuse"]));
			}
			
			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_uber_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "uber";
		material.properties["transmission"].constant.value = spectrum(0);
		material.properties["transmission"].type = "constant";
		material.properties["reflectance"].constant.value = spectrum(0);
		material.properties["reflectance"].type = "constant";
		material.properties["specular"].constant.value = spectrum(0.25f);
		material.properties["specular"].type = "constant";
		material.properties["diffuse"].constant.value = spectrum(0.25f);
		material.properties["diffuse"].type = "constant";
		material.properties["opacity"].constant.value = spectrum(1);
		material.properties["opacity"].type = "constant";
		material.properties["roughness"].constant.value = spectrum(0.1f);
		material.properties["roughness"].type = "constant";
		material.properties["eta"].constant.value = spectrum(1.5f);
		material.properties["eta"].type = "constant";
		material.remapping = true;
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"], 0));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"], 1));
				
				if (name == "index") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["eta"]));

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"]));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(material.properties["specular"] = context.state.find_texture(value));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(material.properties["diffuse"] = context.state.find_texture(value));

				if (name == "Kt") META_SCENE_FINISHED_AND_CONTINUE(material.properties["transmission"] = context.state.find_texture(value));
				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(material.properties["reflectance"] = context.state.find_texture(value));

				if (name == "opacity") META_SCENE_FINISHED_AND_CONTINUE(material.properties["opacity"] = context.state.find_texture(value));

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(material.properties["roughness"] = context.state.find_texture(value));


				// now we do not support bump map
				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["specular"]));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["diffuse"]));

				if (name == "Kt") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["transmission"]));
				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["reflectance"]));

				if (name == "opacity") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["opacity"]));
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;
			
			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_mirror_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "mirror";
		material.properties["reflectance"].constant.value = spectrum(0.9f);
		material.properties["reflectance"].type = "constant";
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["reflectance"]));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);
				
				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}
			
			// material name
			if (type == PBRT_STRING_TOKEN) continue;
			
			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_substrate_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "substrate";
		material.properties["specular"].constant.value = spectrum(0.5f);
		material.properties["specular"].type = "constant";
		material.properties["diffuse"].constant.value = spectrum(0.5f);
		material.properties["diffuse"].type = "constant";
		material.properties["roughness"].constant.value = spectrum(0.1f);
		material.properties["roughness"].type = "constant";
		material.remapping = true;
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(material.properties["specular"] = context.state.find_texture(value));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(material.properties["diffuse"] = context.state.find_texture(value));

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(material.properties["roughness"] = context.state.find_texture(value));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(material.properties["roughness"] = context.state.find_texture(value));

				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["specular"]));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["diffuse"]));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"], 0));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"], 1));

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"]));
			}

			if (type == PBRT_BOOL_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "remaproughness") META_SCENE_FINISHED_AND_CONTINUE(material.remapping = string_to_bool(value));
			}
			
			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_fourier_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		warn("pbrt importer : fourier material is not supported. we will create default matte material.");

		material.type = "diffuse";
		material.properties["reflectance"].constant.value = spectrum(0);
		material.properties["reflectance"].type = "constant";
		material.properties["sigma"].constant.value = spectrum(0);
		material.properties["sigma"].type = "constant";
	}

	void import_translucent_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "translucent";
		material.properties["transmission"].constant.value = spectrum(0.5f);
		material.properties["transmission"].type = "constant";
		material.properties["reflectance"].constant.value = spectrum(0.5f);
		material.properties["reflectance"].type = "constant";
		material.properties["specular"].constant.value = spectrum(0.25f);
		material.properties["specular"].type = "constant";
		material.properties["diffuse"].constant.value = spectrum(0.25f);
		material.properties["diffuse"].type = "constant";
		material.properties["roughness"].constant.value = spectrum(0.1f);
		material.properties["roughness"].type = "constant";
		material.remapping = true;
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "transmit") META_SCENE_FINISHED_AND_CONTINUE(material.properties["transmission"] = context.state.find_texture(value));
				if (name == "reflect") META_SCENE_FINISHED_AND_CONTINUE(material.properties["reflectance"] = context.state.find_texture(value));
				
				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(material.properties["specular"] = context.state.find_texture(value));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(material.properties["diffuse"] = context.state.find_texture(value));

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(material.properties["roughness"] = context.state.find_texture(value));
				
				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "transmit") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["transmission"]));
				if (name == "reflect") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["reflectance"]));
				
				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["specular"]));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["diffuse"]));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"]));
			}

			if (type == PBRT_BOOL_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "remaproughness") META_SCENE_FINISHED_AND_CONTINUE(material.remapping = string_to_bool(value));
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	std::shared_ptr<meta_scene::objects::material> convert(const std::optional<meta_scene::objects::material>& material)
	{
		if (!material.has_value()) return nullptr;

		return std::make_shared<meta_scene::objects::material>(material.value());
	}
	
	void import_mix_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "mixture";
		
		material.mixture.material0 = std::make_shared<meta_scene::objects::material>();
		material.mixture.material1 = std::make_shared<meta_scene::objects::material>();
		material.mixture.alpha.constant.value = spectrum(0.5f);
		material.mixture.alpha.type = "constant";
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "amount") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.mixture.alpha));
			}
			
			if (type == PBRT_STRING_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "namedmaterial1") META_SCENE_FINISHED_AND_CONTINUE(material.mixture.material0 = convert(context.state.find_material(value)));
				if (name == "namedmaterial2") META_SCENE_FINISHED_AND_CONTINUE(material.mixture.material1 = convert(context.state.find_material(value)));
				if (name == "type") continue;
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "amount") META_SCENE_FINISHED_AND_CONTINUE(material.mixture.alpha = context.state.find_texture(value));
			}

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}

		if (material.mixture.material0->type == "unknown" || material.mixture.material1->type == "unknown")
			error("pbrt importer : the material in mixture material can not be nullptr.");
	}

	void import_kd_subsurface_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "subsurface";
		material.properties["transmission"].constant.value = spectrum(1);
		material.properties["transmission"].type = "constant";
		material.properties["reflectance"].constant.value = spectrum(1);
		material.properties["reflectance"].type = "constant";
		material.properties["diffuse"].constant.value = spectrum(0.5f);
		material.properties["diffuse"].type = "constant";
		material.properties["mfp"].constant.value = spectrum(1);
		material.properties["mfp"].type = "constant";
		material.properties["roughness"].constant.value = spectrum(0);
		material.properties["roughness"].type = "constant";
		material.properties["eta"].constant.value = spectrum(1.33f);
		material.properties["eta"].type = "constant";
		material.remapping = true;

		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "eta") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["eta"]));
				if (name == "mfp") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["mfp"]));
				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"], 0));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.properties["roughness"], 1));
			}

			if (type == PBRT_STRING_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "type") continue;
			}

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["diffuse"]));
				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["reflectance"]));
				if (name == "mfp") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.properties["mfp"]));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(material.properties["diffuse"] = context.state.find_texture(value));
			}

			if (type == PBRT_BOOL_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "remaproughness") META_SCENE_FINISHED_AND_CONTINUE(material.remapping = string_to_bool(value));
			}
			
			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}
	
	void import_material_from_property_group(scene_context& context, const property_group& properties, 
		std::optional<meta_scene::objects::material>& material)
	{
		const auto type = properties.find(type_and_name(PBRT_STRING_TOKEN, "type"))->second;

		material = meta_scene::objects::material();
		
		if (type == "translucent") import_translucent_material(context, properties, material.value());
		if (type == "kdsubsurface") import_kd_subsurface_material(context, properties, material.value());
		if (type == "conductor") import_conductor_material(context, properties, material.value());
		if (type == "substrate") import_substrate_material(context, properties, material.value());
		if (type == "diffuse") import_diffuse_material(context, properties, material.value());
		if (type == "plastic") import_plastic_material(context, properties, material.value());
		if (type == "fourier") import_fourier_material(context, properties, material.value());
		if (type == "mirror") import_mirror_material(context, properties, material.value());
		if (type == "glass") import_glass_material(context, properties, material.value());
		if (type == "metal") import_metal_material(context, properties, material.value());
		if (type == "matte") import_matte_material(context, properties, material.value());
		if (type == "uber") import_uber_material(context, properties, material.value());
		if (type == "mix") import_mix_material(context, properties, material.value());
		if (type == "none") { material = std::nullopt; return; }
		
		META_SCENE_IMPORT_SUCCESS_CHECK_TYPE(material.value());
	}
	
	void import_material(scene_context& context, std::optional<meta_scene::objects::material>& material)
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

		if (type == "") { material = std::nullopt; return; }

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

	void import_named_material(scene_context& context, std::optional<meta_scene::objects::material>& material)
	{
		const auto name = read_string_from_token(context.peek_one_token());

		material = context.state.find_material(name);
	}
}

#endif