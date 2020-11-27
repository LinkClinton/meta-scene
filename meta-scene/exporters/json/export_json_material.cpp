#include "export_json_material.hpp"

#include "../../materials/diffuse_material.hpp"
#include "../../logs.hpp"


#ifdef __JSON_EXPORTER__

namespace metascene::exporters::json {

	nlohmann::json export_diffuse_material_to_json(const std::shared_ptr<diffuse_material>& material)
	{
		nlohmann::json material_json;

		const auto diffuse = 
			std::static_pointer_cast<color_spectrum>(
				std::static_pointer_cast<constant_texture>(material->reflectance)->spectrum);
		
		material_json["type"] = "diffuse";
		material_json["diffuse"] = { diffuse->red, diffuse->green, diffuse->blue };

		return material_json;
	}
	
	nlohmann::json export_material_to_json(const std::shared_ptr<material>& material)
	{
		if (material->type == materials::type::diffuse)
			return export_diffuse_material_to_json(std::static_pointer_cast<diffuse_material>(material));

		logs::error("unknown material in json exporter.");

		return {};
	}
	
}

#endif
