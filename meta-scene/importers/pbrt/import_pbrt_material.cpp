#include "import_pbrt_material.hpp"

#include "import_pbrt_texture.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	constexpr auto META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT = "pbrt importer : bump map is not supported.";
	
	void import_plastic_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "plastic";
		material.plastic.specular.constant.value = spectrum(0.25f);
		material.plastic.specular.type = "constant";
		material.plastic.diffuse.constant.value = spectrum(0.25f);
		material.plastic.diffuse.type = "constant";
		material.plastic.roughness.constant.value = spectrum(0.1f);
		material.plastic.roughness.type = "constant";
		material.plastic.eta.constant.value = spectrum(1.5f);
		material.plastic.eta.type = "constant";
		material.plastic.remapping = true;
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.plastic.diffuse));
				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.plastic.specular));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(material.plastic.specular = context.state.find_texture(value));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(material.plastic.diffuse = context.state.find_texture(value));

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(material.plastic.roughness = context.state.find_texture(value));
				
				// now we do not support it.
				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}
			
			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.plastic.roughness));
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_glass_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "glass";
		material.glass.transmission.constant.value = spectrum(1);
		material.glass.transmission.type = "constant";
		material.glass.reflectance.constant.value = spectrum(1);
		material.glass.reflectance.type = "constant";
		material.glass.roughness_u.constant.value = spectrum(0);
		material.glass.roughness_u.type = "constant";
		material.glass.roughness_v.constant.value = spectrum(0);
		material.glass.roughness_v.type = "constant";
		material.glass.eta.constant.value = spectrum(1.5f);
		material.glass.eta.type = "constant";
		material.glass.remapping = true;
		
		for (const auto& property : properties)
		{
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.glass.reflectance));
				if (name == "Kt") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.glass.transmission));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.glass.roughness_u));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.glass.roughness_v));
				if (name == "index") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.glass.eta));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);
				
				// now we do not support it.
				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_BOOL_TOKEN) {
				const auto value = string_to_bool(read_string_from_token(property.second));

				if (name == "remaproughness") META_SCENE_FINISHED_AND_CONTINUE(material.glass.remapping = value);
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_metal_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "metal";
		material.metal.eta.constant.value = spectrum(0);
		material.metal.eta.type = "constant";
		material.metal.k.constant.value = spectrum(0);
		material.metal.k.type = "constant";
		material.metal.roughness_u.constant.value = spectrum(0.01f);
		material.metal.roughness_u.type = "constant";
		material.metal.roughness_v.constant.value = spectrum(0.01f);
		material.metal.roughness_v.type = "constant";
		material.metal.remapping = true;
		
		for (const auto& property : properties)
		{
			auto [type, name] = property.first;

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.metal.roughness_u));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.metal.roughness_v));

				if (name == "roughness") {
					import_real_texture(value, material.metal.roughness_u);
					import_real_texture(value, material.metal.roughness_v);

					continue;
				}
			}

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "eta") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.metal.eta));
				if (name == "k") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.metal.k));
			}

			if (type == PBRT_SPECTRUM_TOKEN) {
				const auto value = read_string_from_token(property.second);
				
				if (name == "eta") META_SCENE_FINISHED_AND_CONTINUE(import_sampled_spectrum_texture(context.directory_path + value, material.metal.eta));
				if (name == "k") META_SCENE_FINISHED_AND_CONTINUE(import_sampled_spectrum_texture(context.directory_path + value, material.metal.k));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "roughness") {
					material.metal.roughness_u = context.state.find_texture(value);
					material.metal.roughness_v = context.state.find_texture(value);

					continue;
				}

				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_BOOL_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "remaproughness") META_SCENE_FINISHED_AND_CONTINUE(material.metal.remapping = string_to_bool(value));
			}
			
			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_matte_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "diffuse";

		material.diffuse.reflectance.constant.value = spectrum(0.5f);
		material.diffuse.reflectance.type = "constant";
		material.diffuse.sigma.constant.value = spectrum(0);
		material.diffuse.sigma.type = "constant";
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.diffuse.reflectance));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "sigma") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.diffuse.sigma));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(material.diffuse.reflectance = context.state.find_texture(value));
				if (name == "sigma") META_SCENE_FINISHED_AND_CONTINUE(material.diffuse.sigma = context.state.find_texture(value));
				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_SPECTRUM_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_sampled_spectrum_texture(context.directory_path + value, material.diffuse.reflectance));
			}
			
			// material name
			if (type == PBRT_STRING_TOKEN) continue;

			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_uber_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "uber";
		material.uber.transmission.constant.value = spectrum(0);
		material.uber.transmission.type = "constant";
		material.uber.reflectance.constant.value = spectrum(0);
		material.uber.reflectance.type = "constant";
		material.uber.specular.constant.value = spectrum(0.25f);
		material.uber.specular.type = "constant";
		material.uber.diffuse.constant.value = spectrum(0.25f);
		material.uber.diffuse.type = "constant";
		material.uber.opacity.constant.value = spectrum(1);
		material.uber.opacity.type = "constant";
		material.uber.roughness_u.constant.value = spectrum(0.1f);
		material.uber.roughness_u.type = "constant";
		material.uber.roughness_v.constant.value = spectrum(0.1f);
		material.uber.roughness_v.type = "constant";
		material.uber.eta.constant.value = spectrum(1.5f);
		material.uber.eta.type = "constant";
		material.uber.remapping = true;
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.uber.roughness_u));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.uber.roughness_v));
				
				if (name == "index") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.uber.eta));

				if (name == "roughness") {
					import_real_texture(value, material.uber.roughness_u);
					import_real_texture(value, material.uber.roughness_v);

					continue;
				}
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(material.uber.specular = context.state.find_texture(value));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(material.uber.diffuse = context.state.find_texture(value));

				if (name == "Kt") META_SCENE_FINISHED_AND_CONTINUE(material.uber.transmission = context.state.find_texture(value));
				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(material.uber.reflectance = context.state.find_texture(value));

				if (name == "opacity") META_SCENE_FINISHED_AND_CONTINUE(material.uber.opacity = context.state.find_texture(value));

				if (name == "roughness") {

					material.uber.roughness_u = context.state.find_texture(value);
					material.uber.roughness_v = context.state.find_texture(value);
					
					continue;
				}

				// now we do not support bump map
				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.uber.specular));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.uber.diffuse));

				if (name == "Kt") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.uber.transmission));
				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.uber.reflectance));

				if (name == "opacity") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.uber.opacity));
			}

			// material name
			if (type == PBRT_STRING_TOKEN) continue;
			
			META_SCENE_PBRT_UN_RESOLVE_TOKEN;
		}
	}

	void import_mirror_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "mirror";
		material.mirror.reflectance.constant.value = spectrum(0.9f);
		material.mirror.reflectance.type = "constant";
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.mirror.reflectance));
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
		material.substrate.specular.constant.value = spectrum(0.5f);
		material.substrate.specular.type = "constant";
		material.substrate.diffuse.constant.value = spectrum(0.5f);
		material.substrate.diffuse.type = "constant";
		material.substrate.roughness_u.constant.value = spectrum(0.1f);
		material.substrate.roughness_u.type = "constant";
		material.substrate.roughness_v.constant.value = spectrum(0.1f);
		material.substrate.roughness_v.type = "constant";
		material.substrate.remapping = true;
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(material.substrate.specular = context.state.find_texture(value));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(material.substrate.diffuse = context.state.find_texture(value));

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(material.substrate.roughness_u = context.state.find_texture(value));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(material.substrate.roughness_v = context.state.find_texture(value));

				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.substrate.specular));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.substrate.diffuse));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.substrate.roughness_u));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.substrate.roughness_v));

				if (name == "roughness") {
					import_real_texture(value, material.substrate.roughness_u);
					import_real_texture(value, material.substrate.roughness_v);

					continue;
				}
			}

			if (type == PBRT_BOOL_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "remaproughness") META_SCENE_FINISHED_AND_CONTINUE(material.substrate.remapping = string_to_bool(value));
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
		material.diffuse.reflectance.constant.value = spectrum(0);
		material.diffuse.reflectance.type = "constant";
		material.diffuse.sigma.constant.value = spectrum(0);
		material.diffuse.sigma.type = "constant";
	}

	void import_translucent_material(scene_context& context, const property_group& properties, meta_scene::objects::material& material)
	{
		material.type = "translucent";
		material.translucent.transmission.constant.value = spectrum(0.5f);
		material.translucent.transmission.type = "constant";
		material.translucent.reflectance.constant.value = spectrum(0.5f);
		material.translucent.reflectance.type = "constant";
		material.translucent.specular.constant.value = spectrum(0.25f);
		material.translucent.specular.type = "constant";
		material.translucent.diffuse.constant.value = spectrum(0.25f);
		material.translucent.diffuse.type = "constant";
		material.translucent.roughness.constant.value = spectrum(0.1f);
		material.translucent.roughness.type = "constant";
		material.translucent.remapping = true;
		
		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "transmit") META_SCENE_FINISHED_AND_CONTINUE(material.translucent.transmission = context.state.find_texture(value));
				if (name == "reflect") META_SCENE_FINISHED_AND_CONTINUE(material.translucent.reflectance = context.state.find_texture(value));
				
				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(material.translucent.specular = context.state.find_texture(value));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(material.translucent.diffuse = context.state.find_texture(value));

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(material.translucent.roughness = context.state.find_texture(value));
				
				if (name == "bumpmap") META_SCENE_FINISHED_AND_CONTINUE(warn(META_SCENE_PBRT_BUMP_MAP_IS_NOT_SUPPORT));
			}

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "transmit") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.translucent.transmission));
				if (name == "reflect") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.translucent.reflectance));
				
				if (name == "Ks") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.translucent.specular));
				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.translucent.diffuse));
			}

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "roughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.translucent.roughness));
			}

			if (type == PBRT_BOOL_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "remaproughness") META_SCENE_FINISHED_AND_CONTINUE(material.translucent.remapping = string_to_bool(value));
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

		material.subsurface.transmission.constant.value = spectrum(1);
		material.subsurface.transmission.type = "constant";
		material.subsurface.reflectance.constant.value = spectrum(1);
		material.subsurface.reflectance.type = "constant";
		material.subsurface.diffuse.constant.value = spectrum(0.5f);
		material.subsurface.diffuse.type = "constant";
		material.subsurface.mfp.constant.value = spectrum(1);
		material.subsurface.mfp.type = "constant";
		material.subsurface.roughness_u.constant.value = spectrum(0);
		material.subsurface.roughness_u.type = "constant";
		material.subsurface.roughness_v.constant.value = spectrum(0);
		material.subsurface.roughness_v.type = "constant";
		material.subsurface.eta.constant.value = spectrum(1.33f);
		material.subsurface.eta.type = "constant";
		material.subsurface.remapping = true;

		for (const auto& property : properties) {
			auto [type, name] = property.first;

			if (type == PBRT_FLOAT_TOKEN) {
				const auto value = remove_special_character(property.second);

				if (name == "eta") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.subsurface.eta));
				if (name == "mfp") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.subsurface.mfp));
				if (name == "uroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.subsurface.roughness_u));
				if (name == "vroughness") META_SCENE_FINISHED_AND_CONTINUE(import_real_texture(value, material.subsurface.roughness_v));
			}

			if (type == PBRT_STRING_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "type") continue;
			}

			if (type == PBRT_COLOR_TOKEN || type == PBRT_RGB_TOKEN) {
				const auto value = property.second;

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.subsurface.diffuse));
				if (name == "Kr") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.subsurface.reflectance));
				if (name == "mfp") META_SCENE_FINISHED_AND_CONTINUE(import_color_spectrum_texture(value, material.subsurface.mfp));
			}

			if (type == PBRT_TEXTURE_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "Kd") META_SCENE_FINISHED_AND_CONTINUE(material.subsurface.diffuse = context.state.find_texture(value));
			}

			if (type == PBRT_BOOL_TOKEN) {
				const auto value = read_string_from_token(property.second);

				if (name == "remaproughness") META_SCENE_FINISHED_AND_CONTINUE(material.subsurface.remapping = string_to_bool(value));
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
		if (type == "substrate") import_substrate_material(context, properties, material.value());
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