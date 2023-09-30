#include "json_exporter.hpp"

#include "export_json_camera.hpp"
#include "export_json_config.hpp"
#include "export_json_entity.hpp"

#include <fstream>
#include <format>

#ifdef __JSON_EXPORTER__

namespace meta_scene::exporters::json
{
	static std::string space(size_t indent)
	{
		std::string generate_string = "";

		for (size_t index = 0; index < indent; index++)
		{
			generate_string.push_back(' ');
		}

		return generate_string;
	}

	nlohmann::json export_scene(const scene& scene)
	{
		nlohmann::json export_json;

		export_json["config"] = export_config(scene);
		export_json["camera"] = export_camera(scene.camera);
		export_json["film"] = export_film(scene.film);

		for (size_t index = 0; index < scene.entities.size(); index++)
			export_json["entities"].push_back(export_entity(scene.entities[index], "entity" + std::to_string(index)));

		return export_json;
	}

	void print_readable_json_internal(print_context& context, const nlohmann::json& json)
	{
		if (json.is_object())
		{
			context.stream << "{" << std::endl;

			size_t child_count = json.size();
			size_t child_index = 0;

			for (const auto& child : json.items())
			{
				context.indent += 4;
				
				context.stream << space(context.indent) << "\"" << child.key() << "\": ";

				print_readable_json_internal(context, child.value());

				context.stream << ((child_index + 1 == child_count) ? "" : ",") << std::endl;

				context.indent -= 4;
				
				child_index++;
			}

			context.stream << space(context.indent) << "}";
		} else 
		if (json.is_array()) 
		{
			if (json.size() == 2 && json[0].is_number())
			{
				// handle [x, y] number array
				context.stream << std::format("[{}, {}]", json[0].dump(), json[1].dump());
			} else 
			if (json.size() == 3 && json[0].is_number())
			{
				// handle [r, g, b] number array
				context.stream << std::format("[{}, {}, {}]", json[0].dump(), json[1].dump(), json[2].dump());
			} else
			if (json.size() == 16 && json[0].is_number())
			{
				// handle 4x4 matrix number array
				context.stream << "[" << std::endl;
				
				context.stream << space(context.indent + 4) << std::format("{}, {}, {}, {},", json[0].dump(), json[1].dump(), json[2].dump(), json[3].dump()) << std::endl;
				context.stream << space(context.indent + 4) << std::format("{}, {}, {}, {},", json[4].dump(), json[5].dump(), json[6].dump(), json[7].dump()) << std::endl;
				context.stream << space(context.indent + 4) << std::format("{}, {}, {}, {},", json[8].dump(), json[9].dump(), json[10].dump(), json[11].dump()) << std::endl;
				context.stream << space(context.indent + 4) << std::format("{}, {}, {}, {}", json[12].dump(), json[13].dump(), json[14].dump(), json[15].dump()) << std::endl;

				context.stream << space(context.indent) << "]";
			} else
			if (json.size() != 0 && json.size() % 3 == 0 && json[0].is_number_unsigned())
			{
				// handle "indices" unsigned int array
				context.stream << "[" << std::endl;

				for (size_t index = 0; index < json.size(); index += 3)
				{
					context.stream
						<< space(context.indent + 4)
						<< std::format("{}, {}, {}", json[index + 0].dump(), json[index + 1].dump(), json[index + 2].dump())
						<< (index + 3 == json.size() ? "" : ",")
						<< std::endl;
				}

				context.stream << space(context.indent) << "]";
			} else
			{
				context.stream << "[" << std::endl;

				size_t item_count = json.size();
				size_t item_index = 0;

				for (const auto& item : json.items())
				{
					context.indent += 4;

					context.stream << space(context.indent);

					print_readable_json_internal(context, item.value());

					context.stream << ((item_index + 1 == item_count) ? "" : ",") << std::endl;

					context.indent -= 4;

					item_index++;
				}

				context.stream << space(context.indent) << "]";
			}
		}else 
		{
			context.stream << json;
		}
	}

	void print_readable_json(print_context& context, const nlohmann::json& json)
	{
		context.stream << "{" << std::endl;
		context.indent += 4;

		constexpr std::array elements_name = { "camera", "config", "film", "entities" };

		for (size_t index = 0; index < elements_name.size(); index++)
		{
			const auto& element_name = elements_name[index];

			context.stream << space(context.indent) << "\"" << element_name << "\": ";

			print_readable_json_internal(context, json[element_name]);

			context.stream << (index + 1 == elements_name.size() ? "" : ",") << std::endl;
		}

		context.indent -= 4;
		context.stream << "}";
	}

	void export_scene_to_file(const scene& scene, const std::string& filename)
	{
		const auto json = export_scene(scene);

		print_context context;

		context.stream = std::ofstream(filename);
		context.indent = 0;

		print_readable_json(context, json);

		context.stream.close();
	}

}

#endif
