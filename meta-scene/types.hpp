#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#pragma warning(disable : 4275)

#include <spdlog/spdlog.h>

#include <unordered_map>
#include <string>

namespace meta_scene {

	using vector3 = glm::vec3;
	using vector4 = glm::vec4;

	using spectrum = vector3;
	
	using matrix4x4 = glm::mat4x4;

	using real = float;
	using uint32 = unsigned;
	
	template <typename Key, typename Value>
	using mapping = std::unordered_map<Key, Value>;

	template <typename... Args>
	void debug(const std::string& format, const Args& ... args)
	{
		spdlog::debug(format.c_str(), args...);
	}

	template <typename ... Args>
	void info(const std::string& format, const Args&... args)
	{
		spdlog::info(format.c_str(), args...);
	}

	template <typename ... Args>
	void warn(const std::string& format, const Args&... args)
	{
		spdlog::warn(format.c_str(), args...);
	}

	template <typename ... Args>
	void error(const std::string& format, const Args&... args)
	{
		spdlog::error(format, args...);
	}

	inline real string_to_real(const std::string& value)
	{
		if constexpr (sizeof(real) == 4)
			return static_cast<real>(std::stof(value));

		if constexpr (sizeof(real) == 8)
			return static_cast<real>(std::stod(value));

		return 0;
	}

	inline bool string_to_bool(const std::string& value)
	{
		if (value == "true" || value == "True") return true;
		return false;
	}

	template <typename T>
	constexpr bool is_integral() {
		return std::is_integral<T>::value && !std::is_unsigned<T>::value;
	}

	template <typename T>
	constexpr bool is_unsigned_integral() {
		return std::is_integral<T>::value && std::is_unsigned<T>::value;
	}

	template <typename T>
	T string_to_integer(const std::string& value)
	{
		if constexpr (is_integral<T>() && sizeof(T) == 4)
			return std::stoi(value);

		if constexpr (is_unsigned_integral<T>() && sizeof(T) == 4)
			return std::stoul(value);

		if constexpr (is_unsigned_integral<T>() && sizeof(T) == 8)
			return std::stoull(value);

		throw "unknown error";

		return 0;
	}
	
}
