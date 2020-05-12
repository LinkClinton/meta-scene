#pragma once

#include <string>

namespace metascene {

	namespace interfaces {

		struct string_property {
			virtual ~string_property() = default;

			virtual std::string to_string() const noexcept = 0;
		};
		
	}
	
}
