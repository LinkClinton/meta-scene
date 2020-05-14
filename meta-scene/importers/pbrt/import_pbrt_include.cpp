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

	vector3 scene_context::peek_vector3()
	{
		const auto x = string_to_real(remove_special_character(peek_one_token()));
		const auto y = string_to_real(remove_special_character(peek_one_token()));
		const auto z = string_to_real(remove_special_character(peek_one_token()));

		return vector3(x, y, z);
	}

	real scene_context::peek_real()
	{
		return string_to_real(remove_special_character(peek_one_token()));
	}

	void scene_context::loop_important_token(const std::function<void()>& function) const
	{
		while (!token_stack.empty()) {
			// if the token is important token, it means we need finished this important token.
			if (is_important_token(top_token())) break;

			function();
		}
	}

	void scene_context::loop_attribute_token(const std::function<void()>& function) const
	{
		while (token_stack.top() != PBRT_ATTRIBUTE_END_TOKEN)
			function();;
	}

	void scene_context::loop_world_token(const std::function<void()>& function) const
	{
		while (token_stack.top() != PBRT_WORLD_END_TOKEN) 
			function();
	}

	bool is_important_token(const std::string& token)
	{
		return
			token == PBRT_ATTRIBUTE_BEGIN_TOKEN || 
			token == PBRT_ATTRIBUTE_END_TOKEN || 
			token == PBRT_WORLD_BEGIN_TOKEN ||
			token == PBRT_INTEGRATOR_TOKEN ||
			token == PBRT_WORLD_END_TOKEN ||
			token == PBRT_LOOK_AT_TOKEN ||
			token == PBRT_SAMPLER_TOKEN ||
			token == PBRT_ROTATE_TOKEN ||
			token == PBRT_CAMERA_TOKEN ||
			token == PBRT_FILM_TOKEN;
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