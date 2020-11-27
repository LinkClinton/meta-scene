#pragma once

#include "export_json_include.hpp"

#ifdef __JSON_EXPORTER__

namespace metascene::exporters::json {

	nlohmann::json export_material_to_json(const std::shared_ptr<material>& material);
	
}

#endif