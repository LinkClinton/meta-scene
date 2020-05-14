#include "import_pbrt_attribute.hpp"

#include "import_pbrt_transform.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {


	void import_attribute(scene_context& context, std::shared_ptr<entity>& entity)
	{
		entity = std::make_shared<metascene::entity>();

		context.loop_attribute_token([&]()
			{
				const auto important_token = context.peek_one_token();

				if (important_token == PBRT_ROTATE_TOKEN) import_rotate(context, entity->transform);
			});
	}
}

#endif