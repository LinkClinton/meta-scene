#include "sphere.hpp"

metascene::shapes::sphere::sphere() : shape(type::sphere)
{
}

std::string metascene::shapes::sphere::to_string() const noexcept
{
	std::stringstream stream;

	stream << "sphere : " << std::endl;
	stream << "radius = " << radius << std::endl;

	return stream.str();
}
