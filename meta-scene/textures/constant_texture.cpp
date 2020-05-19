#include "constant_texture.hpp"

metascene::textures::constant_texture::constant_texture() : texture(type::constant)
{
}

metascene::textures::constant_texture::constant_texture(metascene::real value) : texture(type::constant), real(value)
{
}

std::string metascene::textures::constant_texture::to_string() const noexcept
{
	std::stringstream stream;

	stream << "constant_texture : " << std::endl;
	stream << "spectrum = " << spectrum->to_string() << std::endl;
	stream << "real     = " << real << std::endl;

	return stream.str();
}
