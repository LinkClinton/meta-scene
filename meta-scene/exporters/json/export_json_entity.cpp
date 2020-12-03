#include "export_json_entity.hpp"

#include "export_json_transform.hpp"
#include "export_json_material.hpp"
#include "export_json_light.hpp"
#include "export_json_shape.hpp"

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json {

	nlohmann::json export_entity(const objects::entity& entity, const std::string& name)
	{
		nlohmann::json export_json;

		export_json["transform"] = export_transform(entity.transform);
		export_json["name"] = name;

		if (entity.material.has_value()) export_json["material"] = export_material(entity.material.value());
		if (entity.light.has_value()) export_json["light"] = export_light(entity.light.value());
		if (entity.shape.has_value()) export_json["shape"] = export_shape(entity.shape.value());

		return export_json;
	}
	
}

#endif