#include "import_pbrt_attribute.hpp"

#include "import_pbrt_light_source.hpp"
#include "import_pbrt_transform.hpp"
#include "import_pbrt_material.hpp"
#include "import_pbrt_shape.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {


	void import_attribute(scene_context& context)
	{	
		auto entity = std::make_shared<metascene::entity>();

		context.loop_attribute_token([&]()
			{
				const auto important_token = context.peek_one_token();

				if (important_token == PBRT_REVERSE_ORIENTATION_TOKEN) context.reverse_orientation ^= true;
			
				if (important_token == PBRT_TRANSLATE_TOKEN) import_translate(context, entity->transform);
			
				if (important_token == PBRT_ROTATE_TOKEN) import_rotate(context, entity->transform);

				if (important_token == PBRT_SCALE_TOKEN) import_scale(context, entity->transform);
			
				if (important_token == PBRT_AREA_LIGHT_SOURCE_TOKEN) import_area_light_source(context, entity->emitter);

				if (important_token == PBRT_LIGHT_SOURCE_TOKEN) import_light_source(context, entity->emitter);

				if (important_token == PBRT_MATERIAL_TOKEN) import_material(context, entity->material);

				if (important_token == PBRT_SHAPE_TOKEN) import_shape(context, entity->shape);

				if (important_token == PBRT_NAMED_MATERIAL_TOKEN) import_named_material(context, entity->material);
			});

		if (entity->shape != nullptr) entity->shape->reverse_orientation = context.reverse_orientation;
		
		// the last token should be PBRT_ATTRIBUTE_END_TOKEN
		context.peek_one_token();

		context.scene->entities.push_back(entity);
	}
}

#endif