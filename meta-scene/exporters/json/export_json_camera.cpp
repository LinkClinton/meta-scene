#include "export_json_camera.hpp"

#include "export_json_transform.hpp"

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json {

	nlohmann::json export_camera(const objects::camera& camera)
	{
		nlohmann::json export_json;

		export_json["type"] = camera.type;
		export_json["transform"] = export_transform(camera.transform);
		export_json["perspective"]["fov"] = camera.perspective.fov;
		export_json["depth_of_field"]["focus"] = camera.depth_of_field.focus;
		export_json["depth_of_field"]["lens"] = camera.depth_of_field.lens;
		export_json["name"] = "camera";
		
		return export_json;
	}

	nlohmann::json export_film(const objects::film& film)
	{
		nlohmann::json export_json;

		export_json["resolution"] = { film.size_x, film.size_y };
		export_json["scale"] = film.scale;
		export_json["name"] = "film";
		
		return export_json;
	}

}

#endif