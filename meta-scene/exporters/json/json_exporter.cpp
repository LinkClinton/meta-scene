#include "json_exporter.hpp"

#include "export_json_camera.hpp"
#include "export_json_config.hpp"
#include "export_json_entity.hpp"

#include <fstream>
#include <iomanip>

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json {

	nlohmann::json export_scene(const scene& scene)
	{
		nlohmann::json export_json;

		export_json["config"] = export_config(scene);
		export_json["camera"] = export_camera(scene.camera);
		export_json["film"] = export_film(scene.film);

		for (size_t index = 0; index < scene.entities.size(); index++)
			export_json["entities"].push_back(export_entity(scene.entities[index], "entity" + std::to_string(index)));

		return export_json;
	}

	void export_scene_to_file(const scene& scene, const std::string& filename)
	{
		const auto json = export_scene(scene);

		std::ofstream stream(filename);

		stream << std::setw(4) << json << std::endl;

		stream.close();
	}

}

#endif
