#pragma once

#include "export_json_include.hpp"

#ifdef __JSON_EXPORTER__

namespace metascene::exporters::json {

	nlohmann::json export_camera_and_film_to_json(
		const std::shared_ptr<camera>& camera, const std::shared_ptr<film>& film);
	
}

#endif