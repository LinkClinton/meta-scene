#include "export_json_transform.hpp"

#ifdef __JSON_EXPORTER__

namespace metascene::exporters::json {

	nlohmann::json export_transform_to_json(const matrix4x4& transform)
	{
		/*
		 * "matrix": [m00 m01 m02 m03 ... m33]
		 */
		nlohmann::json transform_json;

		transform_json["matrix"] = {
			transform[0][0], transform[0][1], transform[0][2], transform[0][3],
			transform[1][0], transform[1][1], transform[1][2], transform[1][3],
			transform[2][0], transform[2][1], transform[2][2], transform[2][3],
			transform[3][0], transform[3][1], transform[3][2], transform[3][3]
		};

		return transform_json;
	}
	
}

#endif