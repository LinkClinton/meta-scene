#pragma once

#define __JSON_EXPORTER__
#ifndef __JSON_EXPORTER__
#else

#include "../../utilities.hpp"
#include "../../scene.hpp"

#include <nlohmann/json.hpp>

namespace metascene::exporters::json {

	nlohmann::json export_meta_scene_to_json(const std::shared_ptr<scene>& scene);

	void export_meta_scene_to_json_file(const std::shared_ptr<scene>& scene, const std::string& filename);
}


#endif