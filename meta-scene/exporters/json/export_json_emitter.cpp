#include "export_json_emitter.hpp"

#ifdef __JSON_EXPORTER__

namespace metascene::exporters::json {

	nlohmann::json export_emitter_to_json(const std::shared_ptr<emitter>& emitter)
	{
		return {};
	}
	
}

#endif