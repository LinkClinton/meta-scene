#include "export_json_camera.hpp"

#include "export_json_transform.hpp"

#include "../../cameras/perspective_camera.hpp"

#ifdef __JSON_EXPORTER__

nlohmann::json metascene::exporters::json::export_camera_and_film_to_json(const std::shared_ptr<camera>& camera,
	const std::shared_ptr<film>& film)
{
	nlohmann::json camera_json;

	camera_json["resolution"] = { film->width, film->height };
	camera_json["transform"] = export_transform_to_json(camera->transform);
	camera_json["type"] = "perspective";
	camera_json["name"] = "camera";
	camera_json["fov"] = std::static_pointer_cast<perspective_camera>(camera)->fov;

	return camera_json;
}

#endif