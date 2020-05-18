#include "metal_material.hpp"

metascene::materials::metal_material::metal_material() : material(type::metal)
{
}

std::string metascene::materials::metal_material::to_string() const noexcept
{
	std::stringstream stream;

	stream << "metal_material : " << std::endl;
	stream << "eta = " << eta->to_string() << std::endl;
	stream << "k = " << k->to_string() << std::endl;
	stream << "roughness uv = [" << roughness_u << ", " << roughness_v << "]" << std::endl;
	stream << "remapped  = " << remapped_roughness_to_alpha << std::endl;

	return stream.str();
}
