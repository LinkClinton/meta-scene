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

	void import_shape(scene_context& context, std::shared_ptr<entity>& entity)
	{
		const auto type = remove_special_character(context.peek_one_token());

		entity = std::make_shared<metascene::entity>();

		if (type == "loopsubdiv") import_triangle_mesh(context, entity->shape);
		if (type == "trianglemesh") import_triangle_mesh(context, entity->shape);
		if (type == "plymesh") import_ply_mesh(context, entity->shape);
		if (type == "sphere") import_sphere(context, entity->shape);

		// build the shape we current state of context
		entity->shape->reverse_orientation = context.state.reverse_orientation;
		entity->transform = context.current().transform;
		entity->material = context.current().material;
		entity->emitter = context.current().emitter;
	}

	void import_objects_to_scene(scene_context& context)
	{
		const auto name = read_string_from_token(context.peek_one_token());

		const auto objects = context.state.objects[name];

		for (const auto& object : objects->entities) {
			auto entity = std::make_shared<metascene::entity>();

			entity->material = object->material;
			entity->emitter = object->emitter;
			entity->shape = object->shape;

			entity->transform = context.current().transform * object->transform;

			context.scene->entities.push_back(entity);
		}
	}

	void import_shape_to_scene(scene_context& context)
	{
		std::shared_ptr<entity> entity;

		import_shape(context, entity);

		context.scene->entities.push_back(entity);
	}

	void import_objects(scene_context& context)
	{
		context.push_config();
		
		const auto name = remove_special_character(context.peek_one_token());
		const auto objects = std::make_shared<pbrt::objects>();
		
		context.loop_objects_token([&]()
			{
				const auto important_token = context.peek_one_token();

				if (important_token == PBRT_SHAPE_TOKEN) {
					std::shared_ptr<entity> entity;

					import_shape(context, entity);

					objects->entities.push_back(entity);
				}
			});

		context.state.objects.insert({ name, objects });
		
		context.peek_one_token();
		context.pop_config();
	}
}

#endif