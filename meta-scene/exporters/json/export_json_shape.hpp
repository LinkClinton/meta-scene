#pragma once

#include "export_json_include.hpp"

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json {

	nlohmann::json export_shape(const objects::shape& shape);
	
}

#endif