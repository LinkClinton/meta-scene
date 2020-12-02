#include "import_pbrt_integrator.hpp"

#ifdef __PBRT_IMPORTER__

namespace meta_scene::importers::pbrt {

	void import_path_integrator(scene_context& context, meta_scene::objects::integrator& integrator)
	{
		integrator.path.threshold = static_cast<real>(1);
		integrator.depth = 5;
		integrator.type = "path";
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_INTEGER_TOKEN) {
					const auto value = context.peek_integer<size_t>();
					
					if (name == "maxdepth") META_SCENE_FINISHED_AND_RETURN(integrator.depth = value);
				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();

					if (name == "rrthreshold") META_SCENE_FINISHED_AND_RETURN(integrator.path.threshold = value);
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}

	void import_vol_path_integrator(scene_context& context, meta_scene::objects::integrator& integrator)
	{
		integrator.path.threshold = static_cast<real>(1);
		integrator.depth = 5;
		integrator.type = "volume_path";
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_INTEGER_TOKEN) {
					const auto value = context.peek_integer<size_t>();

					if (name == "maxdepth") META_SCENE_FINISHED_AND_RETURN(integrator.depth = value);
				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();

					if (name == "rrthreshold") META_SCENE_FINISHED_AND_RETURN(integrator.path.threshold = value);
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}

	void import_photon_mapping_integrator(scene_context& context, meta_scene::objects::integrator& integrator)
	{
		integrator.photon_mapping.iterations = 64;
		integrator.photon_mapping.photons = 0;
		integrator.photon_mapping.radius = 1;
		integrator.depth = 5;
		integrator.type = "photon_mapping";
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_INTEGER_TOKEN) {
					const auto value = context.peek_integer<int>();

					if (name == "maxdepth") META_SCENE_FINISHED_AND_RETURN(integrator.depth = value);
					if (name == "iterations" || name == "numiterations") META_SCENE_FINISHED_AND_RETURN(integrator.photon_mapping.iterations = value);
					if (name == "photonsperiteration") META_SCENE_FINISHED_AND_RETURN(integrator.photon_mapping.photons = value == -1 ? 0 : value);
				}

				if (type == PBRT_FLOAT_TOKEN) {
					const auto value = context.peek_real();

					if (name == "radius") META_SCENE_FINISHED_AND_RETURN(integrator.photon_mapping.radius = value);
				}

				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}

	void import_bidirectional_path_integrator(scene_context& context, meta_scene::objects::integrator& integrator)
	{
		integrator.type = "bidirectional_path";
		integrator.depth = 5;
		
		context.loop_important_token([&]()
			{
				auto [type, name] = context.peek_type_and_name();

				if (type == PBRT_INTEGER_TOKEN) {
					const auto value = context.peek_integer<int>();

					if (name == "maxdepth") META_SCENE_FINISHED_AND_RETURN(integrator.depth = value);
				}
			
				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}
	
	void import_direct_integrator(scene_context& context, meta_scene::objects::integrator& integrator)
	{
		integrator.direct.light_samples = 4;
		integrator.direct.bsdf_samples = 4;
		integrator.depth = 5;
		integrator.type = "direct";
		
		context.loop_important_token([&]()
			{
				META_SCENE_PBRT_UN_RESOLVE_TOKEN;
			});
	}
	
	void import_integrator(scene_context& context, meta_scene::objects::integrator& integrator)
	{
		const auto integrator_type = remove_special_character(context.peek_one_token());

		if (integrator_type == "bdpt") import_bidirectional_path_integrator(context, integrator);
		if (integrator_type == "sppm") import_photon_mapping_integrator(context, integrator);
		if (integrator_type == "directlighting") import_direct_integrator(context, integrator);
		if (integrator_type == "volpath") import_vol_path_integrator(context, integrator);
		if (integrator_type == "path") import_path_integrator(context, integrator);

		META_SCENE_IMPORT_SUCCESS_CHECK_TYPE(integrator);
	}
	
}

#endif