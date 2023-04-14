#include "export_json_material.hpp"

#include "export_json_texture.hpp"

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json {

	nlohmann::json export_common_material(const objects::material& material)
	{
		nlohmann::json export_json;

		export_json["type"] = material.type;
		//export_json["remapping"] = material.remapping;

		for (const auto& property : material.properties)
			export_json[property.first] = export_texture(property.second);

		return export_json;
	}
	
	nlohmann::json export_material(const objects::material& material)
	{
		if (material.type == "translucent") return export_common_material(material);
		if (material.type == "subsurface") return export_common_material(material);
		if (material.type == "substrate") return export_common_material(material);
		if (material.type == "conductor") return export_common_material(material);
		if (material.type == "plastic") return export_common_material(material);
		if (material.type == "diffuse") return export_common_material(material);
		if (material.type == "mirror") return export_common_material(material);
		if (material.type == "glass") return export_common_material(material);
		if (material.type == "metal") return export_common_material(material);
		if (material.type == "uber") return export_common_material(material);
		
		error("unknown material in json exporter.");
		
		return {};
	}
	
}

#endif