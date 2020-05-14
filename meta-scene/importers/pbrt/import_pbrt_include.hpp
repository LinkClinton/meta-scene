#pragma once

#include "pbrt_importer.hpp"

#ifdef __PBRT_IMPORTER__

#include <functional>
#include <stack>

namespace metascene::importers::pbrt {

	const std::string PBRT_INTEGER_TOKEN = "integer";
	const std::string PBRT_STRING_TOKEN = "string";

	const std::string PBRT_INTEGRATOR_TOKEN = "Integrator";
	const std::string PBRT_SAMPLER_TOKEN = "Sampler";
	const std::string PBRT_LOOK_AT_TOKEN = "LookAt";
	const std::string PBRT_FILM_TOKEN = "Film";
	
	struct scene_context {
		std::shared_ptr<scene> scene;
		
		std::stack<std::string> token_stack;

		std::string peek_one_token();

		std::string top_token() const;

		std::tuple<std::string, std::string> peek_type_and_name();

		void loop_important_token(const std::function<void()>& function) const;
	};

	bool is_important_token(const std::string& token);
	
	std::string remove_special_character(const std::string& value);
}

#endif