#include "color_spectrum.hpp"

metascene::spectrums::color_spectrum::color_spectrum() : spectrum(type::color)
{
}

metascene::spectrums::color_spectrum::color_spectrum(real red, real green, real blue) :
	spectrum(type::color), red(red), green(green), blue(blue)
{
}

metascene::spectrums::color_spectrum::color_spectrum(real value) :
	spectrum(type::color), red(value), green(value), blue(value)
{
}

std::string metascene::spectrums::color_spectrum::to_string() const noexcept
{
	std::stringstream stream;

	stream << "[color_spectrum] [red = " << red << ", green = " << green << ", blue = " << blue << "]";

	return stream.str();
}
