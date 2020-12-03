#pragma once

#include "export_json_include.hpp"

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json {

	nlohmann::json export_transform(const matrix4x4& transform);

}

#endif