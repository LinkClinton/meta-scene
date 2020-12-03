#include "export_json_texture.hpp"

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json {

	nlohmann::json export_constant_texture(const objects::texture& texture)
	{
		nlohmann::json export_json;

		export_json["type"] = texture.type;
		export_json["constant"]["value"] = texture.constant.value;

		return export_json;
	}

	nlohmann::json export_mixture_texture(const objects::texture& texture)
	{
		nlohmann::json export_json;

		export_json["type"] = texture.type;
		export_json["mixture"]["texture0"] = export_texture(*texture.mixture.texture0);
		export_json["mixture"]["texture1"] = export_texture(*texture.mixture.texture1);
		export_json["mixture"]["alpha"] = export_texture(*texture.mixture.alpha);

		return export_json;
	}
	
	nlohmann::json export_image_texture(const objects::texture& texture)
	{
		nlohmann::json export_json;

		export_json["type"] = texture.type;
		export_json["image"]["filename"] = texture.image.filename;
		export_json["image"]["gamma"] = texture.image.gamma;

		return export_json;
	}

	nlohmann::json export_scale_texture(const objects::texture& texture)
	{
		nlohmann::json export_json;

		export_json["type"] = texture.type;
		export_json["scale"]["scale"] = export_texture(*texture.scale.scale);
		export_json["scale"]["base"] = export_texture(*texture.scale.base);

		return export_json;
	}
	
	nlohmann::json export_texture(const objects::texture& texture)
	{
		if (texture.type == "constant") return export_constant_texture(texture);
		if (texture.type == "mixture") return export_mixture_texture(texture);
		if (texture.type == "image") return export_image_texture(texture);
		if (texture.type == "scale") return export_scale_texture(texture);
		
		error("unknown texture in json exporter.");

		return {};
	}
	
}

#endif