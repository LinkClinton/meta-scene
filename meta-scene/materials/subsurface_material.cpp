#include "subsurface_material.hpp"

metascene::materials::subsurface_material::subsurface_material() : material(type::subsurface)
{
}

std::string metascene::materials::subsurface_material::to_string() const noexcept
{
	std::stringstream stream;

	stream << "subsurface material : " << std::endl;
	stream << "transmission = " << transmission->to_string() << std::endl;
	stream << "reflectance  = " << reflectance->to_string() << std::endl;
	stream << "sigma_a      = " << sigma_a->to_string() << std::endl;
	stream << "sigma_s      = " << sigma_s->to_string() << std::endl;
	stream << "roughness u  = " << roughness_u->to_string() << std::endl;
	stream << "roughness v  = " << roughness_v->to_string() << std::endl;
	stream << "eta          = " << eta->to_string() << std::endl;
	stream << "scale        = " << scale << std::endl;
	stream << "remapped     = " << remapped_roughness_to_alpha << std::endl;

	return stream.str();
}
