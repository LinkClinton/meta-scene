#pragma once

#include "export_json_include.hpp"

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json {

	nlohmann::json export_config(const scene& scene);

	void print_config(print_context& context, const nlohmann::json& config);
}

#endif