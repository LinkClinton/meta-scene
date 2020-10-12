#pragma once

#include "export_json_include.hpp"

#ifdef __JSON_EXPORTER__

namespace metascene::exporters::json {

	nlohmann::json export_entity_to_json(const std::shared_ptr<entity>& entity, const std::string& name);
	
}

#endif