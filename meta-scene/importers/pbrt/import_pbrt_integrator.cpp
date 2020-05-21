#include "import_pbrt_integrator.hpp"

#include "../../integrators/path_integrator.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	void import_path_integrator(scene_context& context, std::shared_ptr<integrator>& integrator)
	{
		integrator = std::make_shared<path_integrator>();
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_INTEGER_TOKEN) {
					const auto value = context.peek_integer<size_t>();
					
					if (name == "maxdepth") integrator->depth = value;
				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();
				}
			});
	}
	
	void import_integrator(scene_context& context, std::shared_ptr<integrator>& integrator)
	{
		const auto integrator_type = remove_special_character(context.peek_one_token());

		if (integrator_type == "path") import_path_integrator(context, integrator);
	}
	
}

#endif