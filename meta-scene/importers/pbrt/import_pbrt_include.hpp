#pragma once

#include "pbrt_importer.hpp"

#ifdef __PBRT_IMPORTER__

#include <functional>
#include <stack>

#define META_SCENE_PBRT_NOT_SUPPORT throw "meta-scene pbrt not support."
#define META_SCENE_PBRT_ERROR_TOKEN throw "meta-scene pbrt error token."

namespace metascene::importers::pbrt {
	
	const std::string PBRT_INTEGER_TOKEN = "integer";
	const std::string PBRT_STRING_TOKEN = "string";
	const std::string PBRT_FLOAT_TOKEN = "float";
	const std::string PBRT_COLOR_TOKEN = "color";
	const std::string PBRT_POINT_TOKEN = "point";

	const std::string PBRT_ATTRIBUTE_BEGIN_TOKEN = "AttributeBegin";
	const std::string PBRT_ATTRIBUTE_END_TOKEN = "AttributeEnd";
	const std::string PBRT_LIGHT_SOURCE_TOKEN = "LightSource";
	const std::string PBRT_WORLD_BEGIN_TOKEN = "WorldBegin";
	const std::string PBRT_INTEGRATOR_TOKEN = "Integrator";
	const std::string PBRT_TRANSLATE_TOKEN = "Translate";
	const std::string PBRT_WORLD_END_TOKEN = "WorldEnd";
	const std::string PBRT_MATERIAL_TOKEN = "Material";
	const std::string PBRT_SAMPLER_TOKEN = "Sampler";
	const std::string PBRT_ROTATE_TOKEN = "Rotate";
	const std::string PBRT_CAMERA_TOKEN = "Camera";
	const std::string PBRT_LOOK_AT_TOKEN = "LookAt";
	const std::string PBRT_SHAPE_TOKEN = "Shape";
	const std::string PBRT_FILM_TOKEN = "Film";
	
	struct scene_context {
		std::shared_ptr<scene> scene;
		
		std::stack<std::string> token_stack;

		std::string directory_path;
		
		std::string peek_one_token();

		std::string top_token() const;

		std::tuple<std::string, std::string> peek_type_and_name();

		vector3 peek_vector3();

		template <typename T>
		T peek_integer();
		
		real peek_real();

		void loop_important_token(const std::function<void()>& function) const;

		void loop_attribute_token(const std::function<void()>& function) const;
		
		void loop_world_token(const std::function<void()>& function) const;
	};

	bool is_important_token(const std::string& token);

	bool is_special_character(char character);

	bool has_special_character(const std::string& value);

	bool has_one_special_character(const std::string& value);
	
	std::string remove_special_character(const std::string& value);

	void import_token_vector3(const std::string& token, std::vector<vector3>& data);

	void import_token_vector2(const std::string& token, std::vector<vector3>& data);

	void import_token_unsigned(const std::string& token, std::vector<unsigned>& data);
	
	template <typename T>
	T scene_context::peek_integer()
	{
		return string_to_integer<T>(remove_special_character(peek_one_token()));
	}
}

#endif