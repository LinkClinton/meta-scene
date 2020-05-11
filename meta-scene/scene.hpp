#pragma once

#include "integrators/integrator.hpp"
#include "samplers/sampler.hpp"
#include "filters/filter.hpp"
#include "cameras/camera.hpp"
#include "cameras/film.hpp"
#include "entity.hpp"

#include <vector>

namespace metascene {

	using namespace integrators;
	using namespace samplers;
	using namespace filters;
	using namespace cameras;
	
	struct scene final {
		std::vector<entity> entities;

		std::shared_ptr<integrator> integrator;
		std::shared_ptr<sampler> sampler;
		std::shared_ptr<filter> filter;
		std::shared_ptr<camera> camera;
		std::shared_ptr<film> film;

		scene() = default;
	};
	
}
