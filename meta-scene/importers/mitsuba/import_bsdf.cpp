#include "import_bsdf.hpp"

#include "../../materials/diffuse_material.hpp"

#include "import_specturm.hpp"

#ifdef __MITSUBA_IMPORTER__

namespace metascene::importers::mitsuba {

	void import_diffuse_bsdf(const tinyxml2::XMLNode* node, std::shared_ptr<material>& material)
	{
		material = std::make_shared<diffuse_material>();

		loop_all_children(node, [&](const tinyxml2::XMLNode* current)
			{
				if (current->Value() == MITSUBA_SPECTRUM_ELEMENT)
					import_spectrum(current, std::static_pointer_cast<diffuse_material>(material)->reflectance);
			});
	}
	
	void import_bsdf(const tinyxml2::XMLNode* node, const std::shared_ptr<scene_cache>& cache, std::shared_ptr<material>& material)
	{
		const auto element = node->ToElement();
		const auto type = std::string(element->Attribute("type"));
		const auto id = std::string(element->Attribute("id"));

		if (type == "diffuse") import_diffuse_bsdf(node, material);

		if (!id.empty()) cache->materials.insert({ id, material });
	}
}

#endif