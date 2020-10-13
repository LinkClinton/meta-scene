#include "export_json_emitter.hpp"

#include "../../emitters/surface_emitter.hpp"
#include "../../logs.hpp"

#ifdef __JSON_EXPORTER__

namespace metascene::exporters::json {

	nlohmann::json export_surface_emitter_to_json(const std::shared_ptr<surface_emitter>& emitter)
	{
		nlohmann::json emitter_json;

		const auto radiance = std::static_pointer_cast<color_spectrum>(
			emitter->radiance->type != spectrums::type::color ?
			create_color_spectrum_from_sampled(emitter->radiance) : emitter->radiance);
		
		emitter_json["type"] = "surface";
		emitter_json["radiance"] = { radiance->red, radiance->green, radiance->blue };

		return emitter_json;
	}
	
	nlohmann::json export_emitter_to_json(const std::shared_ptr<emitter>& emitter)
	{
		if (emitter->type == emitters::type::surface)
			return export_surface_emitter_to_json(std::static_pointer_cast<surface_emitter>(emitter));

		logs::error("unknown emitter in json exporter.");
		
		return {};
	}
	
}

#endif