#include "import_pbrt_attribute.hpp"

#include "import_pbrt_light_source.hpp"
#include "import_pbrt_transform.hpp"
#include "import_pbrt_material.hpp"
#include "import_pbrt_shape.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {


	void import_attribute(scene_context& context)
	{
		context.push_config();
		
		context.loop_attribute_token([&]()
			{
				const auto important_token = context.peek_one_token();

				if (important_token == PBRT_REVERSE_ORIENTATION_TOKEN) context.state.reverse_orientation ^= true;
			
				if (important_token == PBRT_TRANSLATE_TOKEN) import_translate(context, context.current().transform);
			
				if (important_token == PBRT_ROTATE_TOKEN) import_rotate(context, context.current().transform);

				if (important_token == PBRT_SCALE_TOKEN) import_scale(context, context.current().transform);
			
				if (important_token == PBRT_AREA_LIGHT_SOURCE_TOKEN) import_area_light_source(context, context.current().emitter);

				if (important_token == PBRT_LIGHT_SOURCE_TOKEN) import_light_source(context);

				if (important_token == PBRT_MATERIAL_TOKEN) import_material(context, context.current().material);

				if (important_token == PBRT_SHAPE_TOKEN) import_shape(context);

				if (important_token == PBRT_NAMED_MATERIAL_TOKEN) import_named_material(context, context.current().material);
			});

		// the last token should be PBRT_ATTRIBUTE_END_TOKEN
		context.peek_one_token();
		context.pop_config();
	}
}

#endif