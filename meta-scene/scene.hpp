#pragma once

#include "objects/integrator.hpp"
#include "objects/sampler.hpp"
#include "objects/camera.hpp"
#include "objects/entity.hpp"
#include "objects/film.hpp"

namespace meta_scene {

	struct scene final {
		std::vector<objects::entity> entities;

		objects::integrator integrator;
		objects::sampler sampler;
		objects::camera camera;
		objects::film film;

		scene() = default;
	};
	
}
