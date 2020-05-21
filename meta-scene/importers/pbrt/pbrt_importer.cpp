#include "pbrt_importer.hpp"

#include "import_pbrt_light_source.hpp"
#include "import_pbrt_integrator.hpp"
#include "import_pbrt_attribute.hpp"
#include "import_pbrt_transform.hpp"
#include "import_pbrt_material.hpp"
#include "import_pbrt_texture.hpp"
#include "import_pbrt_include.hpp"
#include "import_pbrt_camera.hpp"
#include "import_pbrt_shape.hpp"

#ifdef __PBRT_IMPORTER__

#include <filesystem>
#include <fstream>

namespace metascene::importers::pbrt {

	std::string read_line_without_comment(const std::string& line)
	{
		std::string ret = "";

		// if we get the '#' we will ignore the rest of line
		// because the rest of line is comment
		for (const auto& character : line) {
			if (character == '#') return ret;

			ret.push_back(character);
		}

		return ret;
	}

	std::string preprocess_scene_file(const std::string& filename)
	{
		auto stream = std::ifstream(filename);

		std::string scene = "";
		std::string line = "";

		// read all lines to skip the '#' line
		while (std::getline(stream, line)) 
			scene += read_line_without_comment(line) + " ";

		stream.close();
		
		return scene;
	}

	std::vector<std::string> convert_scene_file_to_vector(const std::string& directory_path, const std::string& scene)
	{
		auto stream = std::stringstream(scene);

		std::vector<std::string> tokens;
		std::string token;

		auto is_include_token = false;

		while (stream >> token) {
			size_t brackets_count = 0;
			size_t marks_count = 0;

			for (const auto& character : token) {
				if (character == '"') marks_count++;
				if (character == '[') brackets_count++;
				if (character == ']') brackets_count--;
			}

			while (brackets_count != 0 || marks_count % 2 == 1) {
				const char token_part = stream.get();

				if (token_part == '"') marks_count++;
				if (token_part == '[') brackets_count++;
				if (token_part == ']') brackets_count--;

				token.push_back(token_part);
			}

			// if we find a PBRT_INCLUDE_TOKEN, next should be the file name
			// we will read all data from the file and push them to tokens
			if (token == PBRT_INCLUDE_TOKEN) {
				is_include_token = true;

				continue;
			}

			if (is_include_token) {
				is_include_token = false;

				const auto filename = directory_path + read_string_from_token(token);
				const auto include_tokens = 
					convert_scene_file_to_vector(directory_path, preprocess_scene_file(filename));

				tokens.insert(tokens.end(), include_tokens.begin(), include_tokens.end());
				
				continue;
			}

			tokens.push_back(token);
		}

		return tokens;
	}

	std::stack<std::string> convert_scene_file_to_stack(const std::string& directory_path, const std::string& scene)
	{
		std::vector<std::string> tokens = convert_scene_file_to_vector(directory_path, scene);
		std::stack<std::string> stack;
		
		for (auto index = static_cast<int>(tokens.size() - 1); index >= 0; index--) 
			stack.push(tokens[index]);

		return stack;
	}

	void import_world(scene_context& context)
	{
		// push the world state
		context.push_config();
		
		context.loop_world_token([&]()
			{
				const auto token = context.peek_one_token();

				if (token == PBRT_ATTRIBUTE_BEGIN_TOKEN) 
					import_attribute(context);

				// when we read a reverse orientation token in world begin
				// not in attribution, it change the global state
				if (token == PBRT_REVERSE_ORIENTATION_TOKEN) 
					context.state.reverse_orientation ^= true;

				if (token == PBRT_MAKE_NAMED_MATERIAL_TOKEN)
					import_named_material(context);

				if (token == PBRT_MAKE_TEXTURE_TOKEN)
					import_texture(context);

				if (token == PBRT_OBJECT_BEGIN_TOKEN)
					import_objects(context);

				if (token == PBRT_TRANSFORM_BEGIN_TOKEN)
					import_transform(context);

				if (token == PBRT_LIGHT_SOURCE_TOKEN) import_light_source(context);
			
				if (token == PBRT_AREA_LIGHT_SOURCE_TOKEN) import_area_light_source(context, context.current().emitter);

				if (token == PBRT_MATERIAL_TOKEN) import_material(context, context.current().material);
			});

		context.pop_config();
	}

	void import_scene(scene_context& context)
	{
		context.scene = std::make_shared<scene>();

		auto invert_transform = matrix4x4(1);
		
		while (!context.token_stack.empty()) {
			const auto token = context.peek_one_token();

			if (token == PBRT_FILM_TOKEN) import_film(context, context.scene->film);

			if (token == PBRT_SAMPLER_TOKEN) import_sampler(context, context.scene->sampler);

			if (token == PBRT_INTEGRATOR_TOKEN) import_integrator(context, context.scene->integrator);

			if (token == PBRT_LOOK_AT_TOKEN) import_look_at(context, invert_transform);

			if (token == PBRT_SCALE_TOKEN) import_scale(context, invert_transform);

			if (token == PBRT_TRANSLATE_TOKEN) import_translate(context, invert_transform);
			
			if (token == PBRT_CONCAT_TRANSFORM_TOKEN) import_concat_matrix(context, invert_transform);
			
			if (token == PBRT_CAMERA_TOKEN) import_camera(context, context.scene->camera);

			if (token == PBRT_WORLD_BEGIN_TOKEN) import_world(context);
		}

		context.scene->camera->transform = inverse(invert_transform);
	}
	
	std::shared_ptr<scene> import_pbrt_scene(const std::string& filename)
	{
		scene_context context;

		context.directory_path = std::filesystem::path(filename).parent_path().generic_string() + "/";
		context.token_stack = convert_scene_file_to_stack(context.directory_path, preprocess_scene_file(filename));
		
		import_scene(context);

		return context.scene;
	}
}

#endif