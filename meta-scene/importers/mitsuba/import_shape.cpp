#include "import_shape.hpp"

#include "../../shapes/mesh.hpp"

#include "import_transform.hpp"
#include "import_emitter.hpp"

#ifdef __MITSUBA_IMPORTER__

namespace metascene::importers::mitsuba {

	void import_obj_mesh(const tinyxml2::XMLNode* node, const std::shared_ptr<scene_cache>& cache, std::shared_ptr<shape>& shape)
	{
		// <string name = "filename" value = "path"/>
		const auto path = node->ToElement()->Attribute("value");

		shape = std::make_shared<mesh>(mesh_type::obj, cache->directory_path + "/" + path);
	}

	void import_reference_in_shape(const tinyxml2::XMLNode* node, const std::shared_ptr<scene_cache>& cache, const std::shared_ptr<entity>& entity)
	{
		entity->material = cache->materials.at(node->ToElement()->Attribute("id"));
	}
	
	void import_shape(const tinyxml2::XMLNode* node, const std::shared_ptr<scene_cache>& cache)
	{
		const auto type = std::string(node->ToElement()->Attribute("type"));

		// we only support obj mode
		if (type != "obj") return;

		auto entity = std::make_shared<metascene::entity>();

		loop_all_children(node, [&](const tinyxml2::XMLNode* current)
			{
				if (current->Value() == MITSUBA_STRING_ELEMENT)
					import_obj_mesh(current, cache, entity->shape);

				if (current->Value() == MITSUBA_TRANSFORM_ELEMENT)
					import_transform(current, entity->transform);

				if (current->Value() == MITSUBA_REFERENCE_ELEMENT)
					import_reference_in_shape(current, cache, entity);

				if (current->Value() == MITSUBA_EMITTER_ELEMENT)
					import_emitter(current, entity->emitter);
			});

		cache->scene->entities.push_back(entity);
	}
}

#endif