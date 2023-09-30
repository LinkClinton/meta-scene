#pragma once

#include "../../types.hpp"
#include "../../scene.hpp"

#ifndef __JSON_EXPORTER__
#define __JSON_EXPORTER__

#include <nlohmann/json.hpp>
#include <fstream>

namespace meta_scene::exporters::json {

	struct print_context
	{
		std::ofstream stream;

		uint32 indent;
	};

	nlohmann::json export_scene(const scene& scene);

	void export_scene_to_file(const scene& scene, const std::string& filename);
	
}

#endif