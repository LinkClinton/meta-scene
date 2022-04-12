#include "export_json_texture.hpp"

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json {

	nlohmann::json export_constant_texture(const objects::texture& texture)
	{
		nlohmann::json export_json;

		export_json["value"] = texture.constant.value;
		export_json["image"] = "";
		export_json["gamma"] = false;
		
		return export_json;
	}

	nlohmann::json export_mixture_texture(const objects::texture& texture)
	{
		error("json format does not support mixture texture.");

		return {};
	}
	
	nlohmann::json export_image_texture(const objects::texture& texture)
	{
		nlohmann::json export_json;

		export_json["value"] = vector3(1, 1, 1);
		export_json["image"] = texture.image.filename;
		export_json["gamma"] = texture.image.gamma;
		
		return export_json;
	}

	nlohmann::json export_scale_texture(const objects::texture& texture)
	{
		nlohmann::json export_json;

		// only support export image with scale value 
		assert(texture.scale.scale->type == "constant");
		assert(texture.scale.base->type == "image");

		export_json["value"] = texture.scale.scale->constant.value;
		export_json["image"] = texture.scale.base->image.filename;
		export_json["gamma"] = texture.scale.base->image.gamma;
		
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