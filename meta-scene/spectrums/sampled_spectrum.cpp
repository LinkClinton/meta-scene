#include "sampled_spectrum.hpp"

metascene::spectrums::sampled_spectrum::sampled_spectrum() : spectrum(type::sampled)
{
}

std::string metascene::spectrums::sampled_spectrum::to_string() const noexcept
{
	std::stringstream stream;

	stream << "[sampled_spectrum] [";

	for (size_t index = 0; index < lambda.size(); index++) 
		stream << lambda[index] << " : " << value[index] << ", ";

	stream << "]";

	return stream.str();
}
