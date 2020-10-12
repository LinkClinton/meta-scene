#include "json_exporter.hpp"

#include "export_json_camera.hpp"
#include "export_json_entity.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>

#ifdef __JSON_EXPORTER__

namespace metascene::exporters::json {
	
	nlohmann::json export_meta_scene_to_json(const std::shared_ptr<scene>& scene)
	{
		nlohmann::json scene_json;

		scene_json["camera"] = export_camera_and_film_to_json(scene->camera, scene->film);

		for (size_t index = 0; index < scene->entities.size(); index++) 
			scene_json["entities"].push_back(export_entity_to_json(scene->entities[index], "entity" + std::to_string(index)));

		return scene_json;
	}

	void export_meta_scene_to_json_file(const std::shared_ptr<scene>& scene, const std::string& filename)
	{
		const auto json = export_meta_scene_to_json(scene);

		std::ofstream stream(filename);

		stream << std::setw(4) << json << std::endl;
		
		stream.close();
	}

}

#endif