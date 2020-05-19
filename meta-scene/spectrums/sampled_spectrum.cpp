#include "sampled_spectrum.hpp"

#include <fstream>

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

std::shared_ptr<metascene::spectrums::sampled_spectrum> metascene::spectrums::read_sampled_spectrum_from_spd(const std::string& filename)
{
	std::fstream stream(filename);

	auto spectrum = std::make_shared<sampled_spectrum>();
	
	real lambda = 0, value = 0;

	while (stream >> lambda >> value) {
		spectrum->lambda.push_back(lambda);
		spectrum->value.push_back(value);
	}

	stream.close();

	return spectrum;
}
