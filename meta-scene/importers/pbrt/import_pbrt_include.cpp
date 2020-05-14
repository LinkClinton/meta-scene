#include "import_pbrt_include.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	bool is_special_character(char character)
	{
		return character == '[' || character == ']' || character == '"';
	}

	std::string scene_context::peek_one_token()
	{
		const auto token = token_stack.top(); token_stack.pop();

		return token;
	}

	std::string scene_context::top_token() const
	{
		return token_stack.top();
	}

	std::tuple<std::string, std::string> scene_context::peek_type_and_name()
	{
		const auto type = remove_special_character(peek_one_token());
		const auto name = remove_special_character(peek_one_token());

		return { type, name };
	}

	void scene_context::loop_important_token(const std::function<void()>& function) const
	{
		while (!token_stack.empty()) {
			// if the token is important token, it means we need finished this important token.
			if (is_important_token(top_token())) break;

			function();
		}
	}

	bool is_important_token(const std::string& token)
	{
		return
			token == PBRT_FILM_TOKEN || 
			token == PBRT_SAMPLER_TOKEN ||
			token == PBRT_LOOK_AT_TOKEN ||
			token == PBRT_INTEGRATOR_TOKEN;
	}

	std::string remove_special_character(const std::string& value)
	{
		// remove the special character, for example '[', ']', '"'
		std::string ret = "";

		for (const auto& character : value)
			if (!is_special_character(character)) ret.push_back(character);

		return ret;
	}
	
}

#endif