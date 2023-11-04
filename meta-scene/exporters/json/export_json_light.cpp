#include "export_json_light.hpp"

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json {

	nlohmann::json export_directional_light(const objects::light& light)
	{
		nlohmann::json export_json;

		export_json["type"] = light.type;
		export_json["intensity"] = light.intensity;
		export_json["direction"] = light.directional.direction;
		
		return export_json;
	}

	nlohmann::json export_environment_light(const objects::light& light)
	{
		nlohmann::json export_json;

		export_json["type"] = light.type;
		export_json["intensity"] = light.intensity;
		export_json["environment"] = light.environment.image;
		
		return export_json;
	}

	nlohmann::json export_surface_light(const objects::light& light)
	{
		nlohmann::json export_json;

		export_json["type"] = light.type;
		export_json["intensity"] = light.intensity;

		return export_json;
	}
	
	nlohmann::json export_light(const objects::light& light)
	{
		if (light.type == "directional") return export_directional_light(light);
		if (light.type == "environment") return export_environment_light(light);
		if (light.type == "surface") return export_surface_light(light);

		error("unknown light in json exporter.");

		return {};
	}
	
}

#endif
