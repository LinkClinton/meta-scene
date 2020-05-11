#include "mitsuba_importer.hpp"

#include "../integrators/path_integrator.hpp"
#include "../materials/diffuse_material.hpp"
#include "../cameras/perspective_camera.hpp"
#include "../spectrums/sampled_spectrum.hpp"
#include "../spectrums/color_spectrum.hpp"
#include "../samplers/random_sampler.hpp"
#include "../filters/gaussian_filter.hpp"


#include <filesystem>
#include <functional>
#include <sstream>

#ifdef __MITSUBA_IMPORTER__

#include <tinyxml2.h>

const std::string MITSUBA_INTEGRATOR_ELEMENT = "integrator";
const std::string MITSUBA_TRANSLATE_ELEMENT = "translate";
const std::string MITSUBA_TRANSFORM_ELEMENT = "transform";
const std::string MITSUBA_REFERENCE_ELEMENT = "ref";
const std::string MITSUBA_SPECTRUM_ELEMENT = "spectrum";
const std::string MITSUBA_SAMPLER_ELEMENT = "sampler";
const std::string MITSUBA_INTEGER_ELEMENT = "integer";
const std::string MITSUBA_EMITTER_ELEMENT = "emitter";
const std::string MITSUBA_FILTER_ELEMENT = "rfilter";
const std::string MITSUBA_SENSOR_ELEMENT = "sensor";
const std::string MITSUBA_STRING_ELEMENT = "string";
const std::string MITSUBA_SHAPE_ELEMENT = "shape";
const std::string MITSUBA_FLOAT_ELEMENT = "float";
const std::string MITSUBA_FILM_ELEMENT = "film";
const std::string MITSUBA_BSDF_ELEMENT = "bsdf";

using namespace metascene::spectrums;

namespace metascene::importers {

	real read_real(const std::string& value)
	{
		if constexpr (sizeof(real) == 4)
			return static_cast<real>(std::stof(value));

		if constexpr (sizeof(real) == 8)
			return static_cast<real>(std::stod(value));

		return 0;
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
	T read_integer(const std::string& value)
	{
		if constexpr (is_integral<T>() && sizeof(T) == 4)
			return std::stoi(value);

		if constexpr (is_unsigned_integral<T>() && sizeof(T) == 8)
			return std::stoull(value);

		throw "unknown error";
		
		return 0;
	}

	vector3 read_vector3(const std::string& value)
	{
		std::stringstream stream(value);

		vector3 ret;

		for (auto index = 0; index < 3; index++) {
			std::string number; std::getline(stream, number, ',');

			ret[index] = read_real(number);
		}

		return ret;
	}

	std::shared_ptr<spectrum> read_sampled_spectrum(const std::string& value) {
		auto spectrum = std::make_shared<sampled_spectrum>();
		
		std::stringstream stream(value);

		std::string wave;

		while (std::getline(stream, wave, ',')) {
			const auto middle = wave.find(':');
			const auto lambda = read_real(wave.substr(0, middle));
			const auto strength = read_real(wave.substr(middle + 1, wave.length() - middle - 1));

			spectrum->lambda.push_back(lambda);
			spectrum->value.push_back(strength);
		}

		return spectrum;
	}

	void loop_all_children(const tinyxml2::XMLNode* node, const std::function<void(const tinyxml2::XMLNode*)>& function)
	{
		auto current = node->FirstChild();

		while (current != nullptr) {
			function(current);

			current = current->NextSibling();
		}
	}

	void process_float(const tinyxml2::XMLNode* node, real& value)
	{
		// read real for example :
		// <float name = "float_property" value = "1."/>
		value = read_real(node->ToElement()->Attribute("value"));
	}

	template <typename T>
	void process_integer(const tinyxml2::XMLNode* node, T& value)
	{
		// read integer for example :
		// <integer name = "integer_property" value = "1"/>
		value = read_integer<T>(node->ToElement()->Attribute("value"));
	}

	void process_spectrum(const tinyxml2::XMLNode* node, std::shared_ptr<spectrum>& value)
	{
		// read spectrum
		// <spectrum name = "reflectance" value = "length:value, length:value..."/>
		value = read_sampled_spectrum(node->ToElement()->Attribute("value"));
	}

	void process_look_at(const tinyxml2::XMLNode* node, matrix4x4& transform)
	{
		const auto element = node->ToElement();

		const auto origin = read_vector3(element->Attribute("origin"));
		const auto target = read_vector3(element->Attribute("target"));
		const auto up = read_vector3(element->Attribute("up"));

		transform *= inverse(look_at<real>(origin, target, up));
	}

	void process_translate(const tinyxml2::XMLNode* node, matrix4x4& transform)
	{
		const auto element = node->ToElement();

		const auto x = read_real(element->Attribute("x"));
		const auto y = read_real(element->Attribute("y"));
		const auto z = read_real(element->Attribute("z"));

		transform *= translate<real>(vector3(x, y, z));
	}

	void process_transform(const tinyxml2::XMLNode* node, matrix4x4& transform)
	{
		loop_all_children(node, [&](const tinyxml2::XMLNode* current)
			{
				// "look_at" is mitsuba 2.0 name, "lookAt" is mitsuba 0.6 name
				if (current->Value() == std::string("look_at") || current->Value() == std::string("lookAt"))
					process_look_at(current, transform);

				if (current->Value() == MITSUBA_TRANSLATE_ELEMENT)
					process_translate(current, transform);

				//todo: solve other transform
			});
	}

	void process_integrator(const tinyxml2::XMLNode* node, std::shared_ptr<integrator>& integrator)
	{
		// because we only support path integrator, so we do not need process anything
		integrator = std::make_shared<path_integrator>();
	}
	
	void process_sampler(const tinyxml2::XMLNode* node, std::shared_ptr<sampler>& sampler)
	{
		sampler = std::make_shared<random_sampler>();

		// we do not process the sampler type
		// because the mitsuba 2.0 only support independent sampler now
		loop_all_children(node, [&](const tinyxml2::XMLNode* current)
			{
				if (current->Value() == MITSUBA_INTEGER_ELEMENT) {
					const std::string name = current->ToElement()->Attribute("name");

					// if the name is "sampleCount"(mitsuba 0.6) or "sample_count"(mitsuba 2.0)
					// we will read the sample per pixel of scene
					if (name == "sampleCount" || name == "sample_count")
						process_integer(current, sampler->sample_per_pixel);
				}
			});
	}

	void process_filter(const tinyxml2::XMLNode* node, std::shared_ptr<filter>& filter)
	{
		const std::string type = node->ToElement()->Attribute("type");

		if (type == "gaussian") filter = std::make_shared<gaussian_filter>();
	}

	void process_film(const tinyxml2::XMLNode* node, std::shared_ptr<film>& film)
	{
		film = std::make_shared<cameras::film>();
		
		loop_all_children(node, [&](const tinyxml2::XMLNode* current)
			{
				// when the current is integer value, it may be width or height
				if (current->Value() == MITSUBA_INTEGER_ELEMENT) {
					const std::string name = current->ToElement()->Attribute("name");

					if (name == "height") process_integer(current, film->height);
					if (name == "width") process_integer(current, film->width);
				}

				// if we find the filter value, it will be used to create film
				if (current->Value() == MITSUBA_FILTER_ELEMENT)
					process_filter(current, film->filter);
			});
	}

	void process_sensor(const tinyxml2::XMLNode* node, const std::shared_ptr<scene>& scene, std::shared_ptr<camera>& camera)
	{
		const auto element = node->ToElement();

		// now we only support perspective camera.
		// so if there is no perspective camera, we do not process it
		if (element->Attribute("type") != std::string("perspective")) return;

		camera = std::make_shared<perspective_camera>();
		
		loop_all_children(node, [&](const tinyxml2::XMLNode* current)
			{
				// if we find the transform element, it should be the transform from camera to world
				// so we process transform of camera
				if (current->Value() == MITSUBA_TRANSFORM_ELEMENT)
					process_transform(current, camera->transform);

				if (current->Value() == MITSUBA_SAMPLER_ELEMENT)
					process_sampler(current, scene->sampler);

				// process the film element and create a film to info
				if (current->Value() == MITSUBA_FILM_ELEMENT)
					process_film(current, scene->film);

				// if we find the float element and its name is fov
				// we will read a float as fov value(angle)
				if (current->Value() == MITSUBA_FLOAT_ELEMENT && current->ToElement()->Attribute("name") == std::string("fov"))
					process_float(current, std::static_pointer_cast<perspective_camera>(camera)->fov);
			});
	}

	void process_diffuse_bsdf(const tinyxml2::XMLNode* node, std::shared_ptr<material>& material)
	{
		std::shared_ptr<spectrum> diffuse = std::make_shared<color_spectrum>();

		loop_all_children(node, [&](const tinyxml2::XMLNode* current)
			{
				if (current->Value() == MITSUBA_SPECTRUM_ELEMENT)
					process_spectrum(current, diffuse);
			});

		material = std::make_shared<diffuse_material>();
	}
	
	std::shared_ptr<scene> load_mitsuba_scene(const std::string& filename)
	{
	}
}

#endif