#include "export_json_config.hpp"

#ifdef __JSON_EXPORTER__

nlohmann::json metascene::exporters::json::export_config_to_json(const std::shared_ptr<scene>& scene)
{
	nlohmann::json config_json;

	config_json["camera_system"] = "left_hand";
	config_json["output_window"]["enable"] = true;
	config_json["output_window"]["name"] = "view_window";
	config_json["output_window"]["font"] = 20;
	config_json["output_images"]["sdr"] = "image.png";
	config_json["output_images"]["hdr"] = "image.hdr";
	config_json["sample_count"] = scene->sampler->sample_per_pixel;

	return config_json;
}

#endif
