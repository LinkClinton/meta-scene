#include "pbrt_importer.hpp"

#include "import_pbrt_integrator.hpp"
#include "import_pbrt_include.hpp"
#include "import_pbrt_camera.hpp"

#ifdef __PBRT_IMPORTER__

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

		while (stream >> token) tokens.push_back(token);

		for (auto index = static_cast<int>(tokens.size() - 1); index >= 0; index--) 
			stack.push(tokens[index]);

		return stack;
	}

	void import_scene(scene_context& context)
	{
		context.scene = std::make_shared<scene>();

		while (!context.token_stack.empty()) {
			const auto token = context.peek_one_token();

			if (token == PBRT_FILM_TOKEN) import_film(context, context.scene->film);

			if (token == PBRT_SAMPLER_TOKEN) import_sampler(context, context.scene->sampler);

			if (token == PBRT_INTEGRATOR_TOKEN) import_integrator(context, context.scene->integrator);
		}
	}
	
	std::shared_ptr<scene> import_pbrt_scene(const std::string& filename)
	{
		scene_context context;

		context.token_stack = convert_scene_file_to_stack(preprocess_scene_file(filename));
		
		import_scene(context);

		return context.scene;
	}
}

#endif