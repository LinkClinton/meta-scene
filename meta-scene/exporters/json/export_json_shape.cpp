#include "export_json_shape.hpp"

#include "../../shapes/triangles.hpp"
#include "../../logs.hpp"

#ifdef __JSON_EXPORTER__

namespace nlohmann {

	template <>
	struct adl_serializer<std::vector<metascene::vector3>> {
		static void to_json(json& j, const std::vector<metascene::vector3>& value)
		{
			for (size_t index = 0; index < value.size(); index++) {
				j.push_back(value[index].x);
				j.push_back(value[index].y);
				j.push_back(value[index].z);
			}
		}

		static void from_json(const json& j, std::vector<metascene::vector3>& value)
		{
			for (size_t index = 0; index < j.size(); index += 3) {
				value.push_back({
					j[index + 0], j[index + 1], j[index + 2]
				});
			}
		}
	};
	
}

namespace metascene::exporters::json {

	std::vector<float> copy_vector3_array_to_float_array(const std::vector<vector3>& values)
	{
		std::vector<float> ret(values.size() * 3);

		std::memcpy(ret.data(), values.data(), sizeof(float) * ret.size());

		return ret;
	}
	
	nlohmann::json export_triangles_to_json(const std::shared_ptr<triangles>& shape)
	{
		nlohmann::json triangles;

		triangles["type"] = "triangles";

		if (!shape->positions.empty()) triangles["data"]["positions"] = shape->positions;
		if (!shape->normals.empty()) triangles["data"]["normals"] = shape->normals;
		if (!shape->indices.empty()) triangles["data"]["indices"] = shape->indices;
		if (!shape->uvs.empty()) triangles["data"]["uvs"] = shape->uvs;

		return triangles;
	}
	
	nlohmann::json export_shape_to_json(const std::shared_ptr<shape>& shape)
	{
		if (shape->type == shapes::type::triangles)
			return export_triangles_to_json(std::static_pointer_cast<triangles>(shape));

		logs::error("unknown shape in json exporter.");
		
		return {};
	}
}

#endif
