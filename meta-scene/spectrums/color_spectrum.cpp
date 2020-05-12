#include "color_spectrum.hpp"

metascene::spectrums::color_spectrum::color_spectrum() : spectrum(type::color)
{
}

std::string metascene::spectrums::color_spectrum::to_string() const noexcept
{
	std::stringstream stream;

	stream << "[color_spectrum] [red = " << red << ", green = " << green << ", blue = " << blue << "]";

	return stream.str();
}
