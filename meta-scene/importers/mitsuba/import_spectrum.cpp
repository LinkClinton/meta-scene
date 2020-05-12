#include "import_specturm.hpp"

#include "../../spectrums/sampled_spectrum.hpp"

#ifdef __MITSUBA_IMPORTER__

using namespace metascene::spectrums;

namespace metascene::importers::mitsuba {

	std::shared_ptr<spectrum> string_to_sampled_spectrum(const std::string& value)
	{
		auto spectrum = std::make_shared<sampled_spectrum>();

		std::stringstream stream(value);

		std::string wave;

		while (std::getline(stream, wave, ',')) {
			const auto middle = wave.find(':');
			const auto lambda = string_to_real(wave.substr(0, middle));
			const auto strength = string_to_real(wave.substr(middle + 1, wave.length() - middle - 1));
			
			spectrum->lambda.push_back(lambda);
			spectrum->value.push_back(strength);
		}

		return spectrum;
	}
	
	void import_sampled_spectrum(const tinyxml2::XMLNode* node, std::shared_ptr<spectrum>& spectrum)
	{
		/*
		 * <spectrum name = "reflectance" value = "length:value, length:value..."/>
		 */
		spectrum = string_to_sampled_spectrum(node->ToElement()->Attribute("value"));
	}

	void import_spectrum(const tinyxml2::XMLNode* node, std::shared_ptr<spectrum>& spectrum)
	{
		import_sampled_spectrum(node, spectrum);
	}
}

#endif