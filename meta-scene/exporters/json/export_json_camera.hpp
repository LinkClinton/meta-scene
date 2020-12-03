#pragma once

#include "export_json_include.hpp"

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json {

	nlohmann::json export_camera(const objects::camera& camera);

	nlohmann::json export_film(const objects::film& film);
	
}

#endif