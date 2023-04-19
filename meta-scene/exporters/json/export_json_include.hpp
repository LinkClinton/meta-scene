#pragma once

#include "json_exporter.hpp"

#ifdef __JSON_EXPORTER__

namespace nlohmann {

	using vector3 = meta_scene::vector3;
	
	template <>
	struct adl_serializer<vector3> {
		static void to_json(json& json, const vector3& vector)
		{
			json = { vector.x, vector.y, vector.z };
		}

		static void from_json(const json& json, vector3& vector)
		{
			vector = vector3(json[0], json[1], json[2]);
		}
	};

	template <>
	struct adl_serializer<std::vector<vector3>> {
		static void to_json(json& json, const std::vector<vector3>& value)
		{
			for (size_t index = 0; index < value.size(); index++) {
				json[index] = value[index];
			}
		}

		static void from_json(const json& json, std::vector<vector3>& value)
		{
			for (size_t index = 0; index < json.size(); index++) {
				value.push_back(json[index]);
			}
		}
	};
	
}

namespace meta_scene::exporters::json {
	
}

#endif