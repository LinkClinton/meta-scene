#include "../../meta-scene/importers/pbrt/pbrt_importer.hpp"
#include "../../meta-scene/exporters/json/json_exporter.hpp"

int main()
{
	//const auto scene = meta_scene::importers::pbrt::import_pbrt_scene("./../../resources/cornell-box/scene.pbrt");
	//const auto scene = meta_scene::importers::pbrt::import_pbrt_scene("./../../resources/measure-one/frame25.pbrt");
	//const auto scene = meta_scene::importers::pbrt::import_pbrt_scene("./../../resources/classroom/scene.pbrt");
	//const auto scene = meta_scene::importers::pbrt::import_pbrt_scene("./../../resources/glass-plate/scene.pbrt");
	const auto scene = meta_scene::importers::pbrt::import_pbrt_scene("./../../resources/ganesha/ganesha.pbrt");


	meta_scene::exporters::json::export_scene_to_file(scene, "./../../resources/ganesha/ganesha.json");

	system("pause");
}