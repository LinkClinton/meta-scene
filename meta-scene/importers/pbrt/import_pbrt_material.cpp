#include "import_pbrt_material.hpp"

#include "import_pbrt_spectrum.hpp"

#include "../../materials/plastic_material.hpp"
#include "../../materials/glass_material.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	void import_plastic_material(scene_context& context, std::shared_ptr<material>& material)
	{
		auto instance = std::make_shared<plastic_material>();

		instance->eta = static_cast<real>(1.5);
		instance->remapped_roughness_to_alpha = true;
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_COLOR_TOKEN) {
					const auto value = context.peek_one_token();

					if (name == "Kd") import_color_spectrum(value, instance->diffuse);
					if (name == "Ks") import_color_spectrum(value, instance->specular);
				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();

					if (name == "roughness") instance->roughness = value;
				}
			});
		
		material = instance;
	}

	void import_glass_material(scene_context& context, std::shared_ptr<material>& material)
	{
		const auto instance = std::make_shared<glass_material>();

		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_COLOR_TOKEN) {
					const auto value = context.peek_one_token();

					if (name == "Kr") import_color_spectrum(value, instance->reflectance);
					if (name == "Kt") import_color_spectrum(value, instance->transmission);
				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();

					if (name == "uroughness") instance->roughness_u = value;
					if (name == "vroughness") instance->roughness_v = value;
				}
			});
		
		material = instance;
	}

	void import_pbrt_material(scene_context& context, std::shared_ptr<material>& material)
	{
		const auto type = remove_special_character(context.peek_one_token());

		if (type == "plastic") import_plastic_material(context, material);
		if (type == "glass") import_glass_material(context, material);
	}
}

#endif