#include "export_json_shape.hpp"

#include "../../shapes/triangles.hpp"
#include "../../logs.hpp"

#ifdef __JSON_EXPORTER__

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

		if (!shape->positions.empty()) triangles["data"]["positions"] = copy_vector3_array_to_float_array(shape->positions);
		if (!shape->normals.empty()) triangles["data"]["normals"] = copy_vector3_array_to_float_array(shape->normals);
		if (!shape->indices.empty()) triangles["data"]["indices"] = shape->indices;
		if (!shape->uvs.empty()) triangles["data"]["uvs"] = copy_vector3_array_to_float_array(shape->uvs);

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
