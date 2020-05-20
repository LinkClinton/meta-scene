#include "import_pbrt_shape.hpp"

#include "../../shapes/triangles.hpp"
#include "../../shapes/sphere.hpp"
#include "../../shapes/mesh.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	void import_triangle_mesh(scene_context& context, std::shared_ptr<shape>& shape)
	{
		auto instance = std::make_shared<triangles>();

		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_POINT_TOKEN) {
					const auto token = context.peek_one_token();

					if (name == "P") import_token_vector3(token, instance->positions);
				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto token = context.peek_one_token();

					if (name == "uv") import_token_vector2(token, instance->uvs);
					if (name == "st") import_token_vector2(token, instance->uvs);
				}

				if (type == PBRT_INTEGER_TOKEN) {
					const auto token = context.peek_one_token();

					if (name == "indices") import_token_unsigned(token, instance->indices);
				}
			});

		shape = instance;
	}

	void import_ply_mesh(scene_context& context, std::shared_ptr<shape>& shape)
	{
		auto instance = std::make_shared<mesh>();

		instance->mesh_type = mesh_type::ply;
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_STRING_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "filename") instance->filename = context.directory_path + value;
				}
			});

		shape = instance;
	}

	void import_sphere(scene_context& context, std::shared_ptr<shape>& shape)
	{
		auto instance = std::make_shared<sphere>();

		instance->radius = static_cast<real>(1);

		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = string_to_real(remove_special_character(context.peek_one_token()));

					if (name == "radius") instance->radius = value;
				}
			});

		shape = instance;
	}
	
	void import_shape(scene_context& context, std::shared_ptr<shape>& shape)
	{
		const auto type = remove_special_character(context.peek_one_token());

		if (type == "loopsubdiv") import_triangle_mesh(context, shape);
		if (type == "trianglemesh") import_triangle_mesh(context, shape);
		if (type == "plymesh") import_ply_mesh(context, shape);
		if (type == "sphere") import_sphere(context, shape);
	}
}

#endif