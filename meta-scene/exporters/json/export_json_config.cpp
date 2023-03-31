#include "export_json_config.hpp"

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json {

	nlohmann::json export_config(const scene& scene)
	{
		nlohmann::json export_json;
		
		export_json["output_window"]["enable"] = true;
		export_json["output_window"]["name"] = "view_window";
		export_json["output_window"]["font"] = 20;
		export_json["output_window"]["size"] = { scene.film.size_x, scene.film.size_y };

		export_json["output_images"]["hdr"] = "image.hdr";
		export_json["output_images"]["sdr"] = "image.png";

		export_json["sample_count"] = scene.sampler.sample_per_pixel;
		export_json["max_depth"] = scene.integrator.depth;

		return export_json;
	}
	
}

#endif