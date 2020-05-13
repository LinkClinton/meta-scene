#include "import_bsdf.hpp"

#include "../../materials/diffuse_material.hpp"
#include "../../materials/plastic_material.hpp"

#include "import_spectrum.hpp"

#ifdef __MITSUBA_IMPORTER__

namespace metascene::importers::mitsuba {

	void import_diffuse_bsdf(const tinyxml2::XMLNode* node, std::shared_ptr<material>& material)
	{
		material = std::make_shared<diffuse_material>();

		loop_all_children(node, [&](const tinyxml2::XMLNode* current)
			{
				if (current->Value() == MITSUBA_SPECTRUM_ELEMENT)
					import_spectrum(current, std::static_pointer_cast<diffuse_material>(material)->reflectance);

				if (current->Value() == MITSUBA_RGB_ELEMENT)
					import_rgb(current, std::static_pointer_cast<diffuse_material>(material)->reflectance);
			});
	}

	void import_plastic_diffuse_reflectance(const tinyxml2::XMLNode* node, std::shared_ptr<spectrum>& diffuse)
	{
		const auto name = std::string(node->ToElement()->Attribute("name"));

		if (name != "diffuse_reflectance" && name != "diffuseReflectance")
			return;

		if (node->Value() == MITSUBA_RGB_ELEMENT)
			import_rgb(node, diffuse);

		if (node->Value() == MITSUBA_SPECTRUM_ELEMENT)
			import_spectrum(node, diffuse);
	}

	void import_plastic_specular_reflectance(const tinyxml2::XMLNode* node, std::shared_ptr<spectrum>& specular)
	{
		const auto name = std::string(node->ToElement()->Attribute("name"));

		if (name != "specular_reflectance" && name != "specularReflectance")
			return;

		if (node->Value() == MITSUBA_RGB_ELEMENT)
			import_rgb(node, specular);

		if (node->Value() == MITSUBA_SPECTRUM_ELEMENT)
			import_spectrum(node, specular);
	}

	void import_plastic_int_ior(const tinyxml2::XMLNode* node, real& ior)
	{
		const auto name = std::string(node->ToElement()->Attribute("name"));

		if (name != "intIOR" && name != "int_ior") return;

		import_float(node, ior);
	}

	void import_plastic_ext_ior(const tinyxml2::XMLNode* node, real& ior)
	{
		const auto name = std::string(node->ToElement()->Attribute("name"));

		if (name != "extIOR" && name != "ext_ior") return;

		import_float(node, ior);
	}
	
	void import_plastic_alpha(const tinyxml2::XMLNode* node, real& roughness)
	{
		const auto name = std::string(node->ToElement()->Attribute("name"));

		if (name != "alpha") return;

		import_float(node, roughness);
	}
	
	void import_plastic_bsdf(const tinyxml2::XMLNode* node, std::shared_ptr<material>& material)
	{
		material = std::make_shared<plastic_material>();

		auto int_ior = static_cast<real>(1.5);
		auto ext_ior = static_cast<real>(1);
		
		loop_all_children(node, [&](const tinyxml2::XMLNode* current)
			{
				import_plastic_specular_reflectance(current, std::static_pointer_cast<plastic_material>(material)->specular);
				import_plastic_diffuse_reflectance(current, std::static_pointer_cast<plastic_material>(material)->diffuse);
				import_plastic_alpha(current, std::static_pointer_cast<plastic_material>(material)->roughness);
				import_plastic_int_ior(current, int_ior);
				import_plastic_ext_ior(current, ext_ior);
			});

		std::static_pointer_cast<plastic_material>(material)->eta = int_ior / ext_ior;
	}
	
	void import_bsdf(const tinyxml2::XMLNode* node, const std::shared_ptr<scene_cache>& cache, std::shared_ptr<material>& material)
	{
		const auto element = node->ToElement();
		const auto type = std::string(element->Attribute("type"));
		const auto id = element->Attribute("id");

		if (type == "diffuse") import_diffuse_bsdf(node, material);
		if (type == "roughplastic") import_plastic_bsdf(node, material);

		if (id != nullptr) cache->materials.insert({ std::string(id), material });
	}
}

#endif