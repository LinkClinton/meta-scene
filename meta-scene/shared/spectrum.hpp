#pragma once

#include "../types.hpp"

namespace meta_scene::shared {

	spectrum create_sampled_spectrum_from_spd(const std::string& filename);

	spectrum create_sampled_spectrum_from_black_body(real temperature, real scale);
	
}
