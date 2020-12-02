#pragma once

#include "../../scene.hpp"

#define __PBRT_IMPORTER__
#ifndef __PBRT_IMPORTER__
#else

#include <string>

namespace meta_scene::importers::pbrt {

	scene import_pbrt_scene(const std::string& filename);

}

#endif