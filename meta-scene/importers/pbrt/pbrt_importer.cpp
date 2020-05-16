#include "pbrt_importer.hpp"

#include "import_pbrt_integrator.hpp"
#include "import_pbrt_attribute.hpp"
#include "import_pbrt_transform.hpp"
#include "import_pbrt_include.hpp"
#include "import_pbrt_camera.hpp"

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
			scene = scene + read_line_without_comment(line) + " ";

		stream.close();
		
		return scene;
	}

	std::stack<std::string> convert_scene_file_to_stack(const std::string& scene)
	{
		auto stream = std::stringstream(scene);

		std::vector<std::string> tokens;
		std::stack<std::string> stack;
		std::string token;

		while (stream >> token) {
			if (has_one_special_character(token)) {
				char token_part = stream.get();
				
				while (!is_special_character(token_part)) {
					token.push_back(token_part);

					token_part = stream.get();
				}

				token.push_back(token_part);
			}

			tokens.push_back(token);
		}

		for (auto index = static_cast<int>(tokens.size() - 1); index >= 0; index--) 
			stack.push(tokens[index]);

		return stack;
	}

	void import_world(scene_context& context)
	{
		context.loop_world_token([&]()
			{
				std::shared_ptr<entity> entity;
			
				import_attribute(context, entity);

				context.scene->entities.push_back(entity);
			});
	}

	void import_scene(scene_context& context)
	{
		context.scene = std::make_shared<scene>();

		matrix4x4 camera_transform = matrix4x4(1);
		
		while (!context.token_stack.empty()) {
			const auto token = context.peek_one_token();

			if (token == PBRT_FILM_TOKEN) import_film(context, context.scene->film);

			if (token == PBRT_SAMPLER_TOKEN) import_sampler(context, context.scene->sampler);

			if (token == PBRT_INTEGRATOR_TOKEN) import_integrator(context, context.scene->integrator);

			if (token == PBRT_LOOK_AT_TOKEN) import_look_at(context, camera_transform);

			if (token == PBRT_CAMERA_TOKEN) import_camera(context, context.scene->camera);

			if (token == PBRT_WORLD_BEGIN_TOKEN) import_world(context);
		}

		context.scene->camera->transform = camera_transform;
	}
	
	std::shared_ptr<scene> import_pbrt_scene(const std::string& filename)
	{
		scene_context context;

		context.token_stack = convert_scene_file_to_stack(preprocess_scene_file(filename));
		context.directory_path = std::filesystem::path(filename).parent_path().generic_string() + "/";
		
		import_scene(context);

		return context.scene;
	}
}

#endif