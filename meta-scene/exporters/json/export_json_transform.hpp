#pragma once

#include "export_json_include.hpp"

#ifdef __JSON_EXPORTER__

namespace metascene::exporters::json {

	nlohmann::json export_transform_to_json(const matrix4x4& transform);
	
}

#endif