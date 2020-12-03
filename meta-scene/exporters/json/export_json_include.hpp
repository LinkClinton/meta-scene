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
		static void to_json(json& j, const std::vector<vector3>& value)
		{
			for (size_t index = 0; index < value.size(); index++) {
				j.push_back(value[index].x);
				j.push_back(value[index].y);
				j.push_back(value[index].z);
			}
		}

		static void from_json(const json& j, std::vector<vector3>& value)
		{
			for (size_t index = 0; index < j.size(); index += 3) {
				value.push_back({
					j[index + 0], j[index + 1], j[index + 2]
					});
			}
		}
	};
	
}

namespace meta_scene::exporters::json {
	
}

#endif