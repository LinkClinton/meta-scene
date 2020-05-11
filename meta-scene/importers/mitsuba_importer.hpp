#pragma once

#include "../scene.hpp"

#define __MITSUBA_IMPORTER__

namespace metascene {

	namespace importers {

#ifdef __MITSUBA_IMPORTER__

		std::shared_ptr<scene> load_mitsuba_scene(const std::string& filename);
		
#endif
	}
}
