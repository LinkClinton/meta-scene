#include "export_json_shape.hpp"

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json {

	nlohmann::json export_triangles(const objects::shape& shape)
	{
		nlohmann::json export_json;

		// todo : orientation may be not necessary
		//export_json["reverse_orientation"] = shape.reverse_orientation;
		export_json["type"] = shape.type;

		if (!shape.triangles.positions.empty()) export_json["triangles"]["positions"] = shape.triangles.positions;
		if (!shape.triangles.normals.empty()) export_json["triangles"]["normals"] = shape.triangles.normals;
		if (!shape.triangles.indices.empty()) export_json["triangles"]["indices"] = shape.triangles.indices;
		if (!shape.triangles.uvs.empty()) export_json["triangles"]["uvs"] = shape.triangles.uvs;

		return export_json;
	}

	nlohmann::json export_sphere(const objects::shape& shape)
	{
		nlohmann::json export_json;

		//export_json["reverse_orientation"] = shape.reverse_orientation;
		export_json["type"] = shape.type;
		export_json["sphere"]["radius"] = shape.sphere.radius;

		return export_json;
	}

	nlohmann::json export_mesh(const objects::shape& shape)
	{
		nlohmann::json export_json;

		//export_json["reverse_orientation"] = shape.reverse_orientation;
		export_json["type"] = shape.type;
		export_json["mesh"] = shape.mesh.filename;
		
		return export_json;
	}
	
	nlohmann::json export_shape(const objects::shape& shape)
	{
		if (shape.type == "triangles") return export_triangles(shape);
		if (shape.type == "sphere") return export_sphere(shape);
		if (shape.type == "mesh") return export_mesh(shape);

		error("unknown shape in json exporter.");

		return {};
	}
	
}

#endif