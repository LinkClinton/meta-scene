#include "spectrum.hpp"

#include "cie_samples.hpp"

#include <fstream>

namespace meta_scene::shared {

	inline real lerp(const real& s, const real& t, real x)
	{
		return s * (1 - x) + t * x;
	}

	inline real interpolate_spectrum_samples(
		const std::vector<real>& lambda,
		const std::vector<real>& value,
		real x)
	{
		if (x <= lambda.front()) return value.front();
		if (x >= lambda.back()) return value.back();

		const auto offset = static_cast<size_t>(std::lower_bound(lambda.begin(), lambda.end(), x) - lambda.begin());
		const auto t = (x - lambda[offset - 1]) / (lambda[offset] - lambda[offset - 1]);

		return lerp(value[offset - 1], value[offset], t);
	}

	inline spectrum create_color_spectrum_from_xyz(real x, real y, real z)
	{
		spectrum spectrum;

		spectrum.r = +3.240479f * x - 1.537150f * y - 0.498535f * z;
		spectrum.g = -0.969256f * x + 1.875991f * y + 0.041556f * z;
		spectrum.b = +0.055648f * x - 0.204043f * y + 1.057311f * z;

		return spectrum;
	}

	spectrum create_color_spectrum_from_sampled(const std::vector<real>& lambda, const std::vector<real>& value)
	{
		real x = 0, y = 0, z = 0;

		for (size_t index = 0; index < cie_samples; index++) {
			const auto sampled_value = interpolate_spectrum_samples(lambda, value, cie_samples_lambda[index]);

			x = x + sampled_value * cie_samples_x[index];
			y = y + sampled_value * cie_samples_y[index];
			z = z + sampled_value * cie_samples_z[index];
		}

		const auto scale = (cie_samples_lambda[cie_samples - 1] - cie_samples_lambda[0]) / (cie_samples_y_integral * cie_samples);

		x = x * scale;
		y = y * scale;
		z = z * scale;

		return create_color_spectrum_from_xyz(x, y, z);
	}

	spectrum create_sampled_spectrum_from_spd(const std::string& filename)
	{
		std::fstream stream(filename);

		std::vector<real> lambdas;
		std::vector<real> values;
		
		real lambda = 0, value = 0;

		while (stream >> lambda >> value) {
			lambdas.push_back(lambda);
			values.push_back(value);
		}

		stream.close();

		return create_color_spectrum_from_sampled(lambdas, values);
	}

	std::tuple<std::vector<real>, std::vector<real>> black_body(
		const std::vector<real>& lambda, real temperature)
	{
		std::vector<real> values(lambda.size(), 0);
		
		if (temperature <= 0) return { lambda, values };

		constexpr static auto c = static_cast<real>(299792458);
		constexpr static auto h = static_cast<real>(6.62606957e-34);
		constexpr static auto kb = static_cast<real>(1.3806488e-23);

		for (size_t index = 0; index < values.size(); index++) {
			const auto lambda_pow_1 = static_cast<real>(lambda[index] * 1e-9);
			const auto lambda_pow_5 = (lambda_pow_1 * lambda_pow_1) * (lambda_pow_1 * lambda_pow_1) * lambda_pow_1;

			values[index] = (2 * h * c * c) /
				(lambda_pow_5 * (exp((h * c) / (lambda_pow_1 * kb * temperature)) - 1));
		}

		return { lambda, values };
	}

	
	std::tuple<std::vector<real>, std::vector<real>> create_sampled_spectrum_from_black_body(
		const std::vector<real>& lambda, real temperature, real scale)
	{
		auto [lambdas, values] = black_body(lambda, temperature);

		const auto lambda_max = static_cast<real>(2.8977721e-3 / temperature * 1e9);
		const auto max_L = black_body({ lambda_max }, temperature);

		for (size_t index = 0; index < values.size(); index++) {
			values[index] = values[index] / std::get<1>(max_L)[0];
			values[index] = values[index] * scale;
		}

		return { lambdas, values };
	}

	spectrum create_sampled_spectrum_from_black_body(real temperature, real scale)
	{
		auto [lambdas, values] = create_sampled_spectrum_from_black_body(
			std::vector<real>(cie_samples_lambda, cie_samples_lambda + cie_samples),
			temperature, scale);

		return create_color_spectrum_from_sampled(lambdas, values);
	}
}
