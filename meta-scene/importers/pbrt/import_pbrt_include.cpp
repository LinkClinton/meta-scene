#include "import_pbrt_include.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	bool is_special_character(char character)
	{
		return character == '[' || character == ']' || character == '"';
	}

	bool has_special_character(const std::string& value)
	{
		for (const auto& character : value)
			if (is_special_character(character)) return true;

		return false;
	}

	bool has_one_special_character(const std::string& value)
	{
		size_t count = 0;

		for (const auto& character : value)
			if (is_special_character(character)) count++;

		if (count == 1) return true;
		if (count == 2 || count == 0) return false;

		META_SCENE_PBRT_ERROR_TOKEN;
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
		auto stream = std::stringstream(remove_special_character(peek_one_token()));

		std::string type, name;

		stream >> type >> name;
		
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
			function();
	}

	void scene_context::loop_world_token(const std::function<void()>& function) const
	{
		while (token_stack.top() != PBRT_WORLD_END_TOKEN) 
			function();
	}

	bool is_important_token(const std::string& token)
	{
		return
			token == PBRT_REVERSE_ORIENTATION_TOKEN ||
			token == PBRT_MAKE_NAMED_MATERIAL_TOKEN ||
			token == PBRT_AREA_LIGHT_SOURCE_TOKEN ||
			token == PBRT_ATTRIBUTE_BEGIN_TOKEN || 
			token == PBRT_ATTRIBUTE_END_TOKEN ||
			token == PBRT_LIGHT_SOURCE_TOKEN ||
			token == PBRT_WORLD_BEGIN_TOKEN ||
			token == PBRT_INTEGRATOR_TOKEN ||
			token == PBRT_TRANSLATE_TOKEN ||
			token == PBRT_WORLD_END_TOKEN ||
			token == PBRT_MATERIAL_TOKEN ||
			token == PBRT_LOOK_AT_TOKEN ||
			token == PBRT_SAMPLER_TOKEN ||
			token == PBRT_ROTATE_TOKEN ||
			token == PBRT_CAMERA_TOKEN ||
			token == PBRT_SCALE_TOKEN ||
			token == PBRT_SHAPE_TOKEN ||
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

	std::string read_string_from_token(const std::string& token)
	{
		auto in_mark = false;

		std::string ret = "";

		for (const auto& character : token) {
			if (character == '"') { in_mark ^= true; continue; }

			if (in_mark) ret.push_back(character);
		}

		return ret;
	}
	
	void import_token_vector3(const std::string& token, std::vector<vector3>& data)
	{
		auto stream = std::stringstream(remove_special_character(token));

		std::string x, y, z;

		while (stream >> x >> y >> z) {
			data.push_back(vector3(
				string_to_real(x),
				string_to_real(y),
				string_to_real(z)
			));
		}
	}

	void import_token_vector2(const std::string& token, std::vector<vector3>& data)
	{
		auto stream = std::stringstream(remove_special_character(token));

		std::string x, y;

		while (stream >> x >> y) {
			data.push_back(vector3(
				string_to_real(x),
				string_to_real(y),
				0
			));
		}
	}

	void import_token_unsigned(const std::string& token, std::vector<unsigned>& data)
	{
		auto stream = std::stringstream(remove_special_character(token));

		std::string value;

		while (stream >> value)
			data.push_back(string_to_integer<unsigned>(value));
	}

}

#endif