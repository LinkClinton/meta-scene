#include "import_pbrt_shape.hpp"

#include "import_pbrt_attribute.hpp"
#include "import_pbrt_material.hpp"
#include "import_pbrt_texture.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	constexpr auto META_SCENE_PBRT_ALPHA_IS_NOT_SUPPORT = "pbrt importer : alpha texture is not support.";
	constexpr auto META_SCENE_PBRT_SHADOW_ALPHA_IS_NOT_SUPPORT = "pbrt importer : shadow alpha texture is not support.";

	
	void import_triangle_mesh(scene_context& context, meta_scene::objects::shape& shape)
	{
		shape.type = "triangles";
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_POINT_TOKEN) {
					const auto token = context.peek_one_token();

					if (name == "P") META_SCENE_FINISHED_AND_RETURN(import_token_vector3(token, shape.triangles.positions));
				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto token = context.peek_one_token();

					if (name == "uv") META_SCENE_FINISHED_AND_RETURN(import_token_vector2(token, shape.triangles.uvs));
					if (name == "st") META_SCENE_FINISHED_AND_RETURN(import_token_vector2(token, shape.triangles.uvs));
				}

				if (type == PBRT_INTEGER_TOKEN) {
					const auto token = context.peek_one_token();

					if (name == "indices") META_SCENE_FINISHED_AND_RETURN(import_token_unsigned(token, shape.triangles.indices));
					if (name == "nlevels") return;
				}

				if (type == PBRT_NORMAL_TOKEN) {
					const auto token = context.peek_one_token();

					if (name == "N") META_SCENE_FINISHED_AND_RETURN(import_token_vector3(token, shape.triangles.normals));
				}
			
				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}

	void import_ply_mesh(scene_context& context, meta_scene::objects::shape& shape)
	{
		shape.type = "mesh";
		shape.mesh.type = "ply";
		shape.mesh.filename = "";
		shape.mesh.mask = std::nullopt;
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_STRING_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "filename") META_SCENE_FINISHED_AND_RETURN(shape.mesh.filename = value);
				}

				if (type == PBRT_TEXTURE_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "alpha") META_SCENE_FINISHED_AND_RETURN(shape.mesh.mask = context.state.find_texture(value));
					if (name == "shadowalpha") META_SCENE_FINISHED_AND_RETURN(shape.mesh.mask = context.state.find_texture(value));
				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = remove_special_character(context.peek_one_token());

					if (name == "alpha") META_SCENE_FINISHED_AND_RETURN(import_real_texture(value, shape.mesh.mask.value()));
					if (name == "shadowalpha") META_SCENE_FINISHED_AND_RETURN(import_real_texture(value, shape.mesh.mask.value()));
				}
			
				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}

	void import_obj_mesh(scene_context& context, meta_scene::objects::shape& shape)
	{
		shape.type = "mesh";
		shape.mesh.type = "obj";
		shape.mesh.filename = "";
		shape.mesh.mask = std::nullopt;

		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_STRING_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "filename") META_SCENE_FINISHED_AND_RETURN(shape.mesh.filename = value);
				}

				if (type == PBRT_TEXTURE_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "alpha") META_SCENE_FINISHED_AND_RETURN(shape.mesh.mask = context.state.find_texture(value));
					if (name == "shadowalpha") META_SCENE_FINISHED_AND_RETURN(shape.mesh.mask = context.state.find_texture(value));
				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = remove_special_character(context.peek_one_token());

					if (name == "alpha") META_SCENE_FINISHED_AND_RETURN(import_real_texture(value, shape.mesh.mask.value()));
					if (name == "shadowalpha") META_SCENE_FINISHED_AND_RETURN(import_real_texture(value, shape.mesh.mask.value()));
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}

	void import_sphere(scene_context& context, meta_scene::objects::shape& shape)
	{
		shape.type = "sphere";
		shape.sphere.radius = 1;
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = string_to_real(remove_special_character(context.peek_one_token()));

					if (name == "radius") META_SCENE_FINISHED_AND_RETURN(shape.sphere.radius = value);
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}

	void import_disk(scene_context& context, meta_scene::objects::shape& shape)
	{
		shape.type = "disk";
		shape.disk.radius = 1;
		shape.disk.height = 0;
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();

					if (name == "radius") META_SCENE_FINISHED_AND_RETURN(shape.disk.radius = value);
					if (name == "height") META_SCENE_FINISHED_AND_RETURN(shape.disk.height = value);
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}

	void import_curve(scene_context& context, meta_scene::objects::shape& shape)
	{
		shape.type = "curve";
		shape.curve.control_points = {};
		shape.curve.width = { 1, 1 };
		shape.curve.u_min = 0;
		shape.curve.u_max = 1;
		
		std::vector<vector3> control_points;
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_STRING_TOKEN) {
					const auto value = read_string_from_token(context.peek_one_token());

					if (name == "type" && value == "cylinder") return;
				}

				if (type == PBRT_POINT_TOKEN) {
					const auto value = context.peek_one_token();

					if (name == "P") META_SCENE_FINISHED_AND_RETURN(import_token_vector3(value, control_points));
				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();

					if (name == "width0") META_SCENE_FINISHED_AND_RETURN(shape.curve.width[0] = value);
					if (name == "width1") META_SCENE_FINISHED_AND_RETURN(shape.curve.width[1] = value);
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		shape.curve.control_points = { control_points[0], control_points[1], control_points[2], control_points[3] };
	}

	void import_shape(scene_context& context, meta_scene::objects::entity& entity)
	{
		const auto type = remove_special_character(context.peek_one_token());

		entity.shape = meta_scene::objects::shape();
		
		if (type == "loopsubdiv") import_triangle_mesh(context, entity.shape.value());
		if (type == "trianglemesh") import_triangle_mesh(context, entity.shape.value());
		if (type == "plymesh") import_ply_mesh(context, entity.shape.value());
		if (type == "objmesh") import_obj_mesh(context, entity.shape.value());
		if (type == "sphere") import_sphere(context, entity.shape.value());
		if (type == "curve") import_curve(context, entity.shape.value());
		if (type == "disk") import_disk(context, entity.shape.value());
		
		// build the shape we current state of context
		entity.shape->reverse_orientation = context.current().reverse_orientation;
		entity.transform = context.current().transform;
		entity.material = context.current().material;
		entity.light = context.current().light;
		entity.media = context.current().media;

		META_SCENE_IMPORT_SUCCESS_CHECK_TYPE(entity.shape.value());
	}

	void import_shape_to(scene_context& context)
	{
		meta_scene::objects::entity entity;

		import_shape(context, entity);

		if (context.current().objects != nullptr)
			context.current().objects->entities.push_back(entity);
		else
			context.scene.entities.push_back(entity);
		
		META_SCENE_IMPORT_SUCCESS_CHECK_TYPE(entity.shape.value());
	}

	void import_objects_to(scene_context& context)
	{
		const auto name = read_string_from_token(context.peek_one_token());

		const auto objects = context.state.find_object(name);

		for (const auto& object : objects.entities) {
			meta_scene::objects::entity entity;
			
			entity.material = object.material;
			entity.light = object.light;
			entity.shape = object.shape;
			entity.media = object.media;

			entity.transform = context.current().transform * object.transform;

			if (context.current().objects != nullptr)
				context.current().objects->entities.push_back(entity);
			else
				context.scene.entities.push_back(entity);
		}
	}

	void import_objects(scene_context& context)
	{
		const auto name = remove_special_character(context.peek_one_token());

		context.push_config();		
		context.current().objects = std::make_shared<objects>();
		
		context.loop_objects_token([&]()
			{
				const auto important_token = context.peek_one_token();

				if (important_token == PBRT_SHAPE_TOKEN) META_SCENE_FINISHED_AND_RETURN(import_shape_to(context));

				if (important_token == PBRT_ATTRIBUTE_BEGIN_TOKEN) META_SCENE_FINISHED_AND_RETURN(import_attribute(context));

				if (important_token == PBRT_MATERIAL_TOKEN) META_SCENE_FINISHED_AND_RETURN(import_material(context, context.current().material));
			
				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});

		context.state.objects.insert({ name, *context.current().objects });
		
		context.peek_one_token();
		context.pop_config();
	}
}

#endif