#include "import_pbrt_attribute.hpp"

#include "import_pbrt_light_source.hpp"
#include "import_pbrt_transform.hpp"
#include "import_pbrt_material.hpp"
#include "import_pbrt_shape.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {


	void import_attribute(scene_context& context, std::shared_ptr<entity>& entity)
	{	
		entity = std::make_shared<metascene::entity>();

		// the first token should be PBRT_ATTRIBUTE_BEGIN_TOKEN
		context.peek_one_token();
		
		context.loop_attribute_token([&]()
			{
				const auto important_token = context.peek_one_token();

				if (important_token == PBRT_TRANSLATE_TOKEN) import_translate(context, entity->transform);
			
				if (important_token == PBRT_ROTATE_TOKEN) import_rotate(context, entity->transform);

				if (important_token == PBRT_LIGHT_SOURCE_TOKEN) import_light_source(context, entity->emitter);

				if (important_token == PBRT_MATERIAL_TOKEN) import_pbrt_material(context, entity->material);

				if (important_token == PBRT_SHAPE_TOKEN) import_shape(context, entity->shape);
			});

		// the last token should be PBRT_ATTRIBUTE_END_TOKEN
		context.peek_one_token();
	}
}

#endif