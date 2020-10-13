#include "export_json_entity.hpp"

#include "export_json_transform.hpp"
#include "export_json_emitter.hpp"
#include "export_json_shape.hpp"

#ifdef __JSON_EXPORTER__

namespace metascene::exporters::json {

	nlohmann::json export_entity_to_json(const std::shared_ptr<entity>& entity, const std::string& name)
	{
		nlohmann::json entity_json;

		entity_json["transform"] = export_transform_to_json(entity->transform);
		entity_json["name"] = name;

		if (entity->emitter != nullptr) entity_json["emitter"] = export_emitter_to_json(entity->emitter);
		if (entity->shape != nullptr) entity_json["shape"] = export_shape_to_json(entity->shape);
		
		return entity_json;
	}
	
}

#endif
