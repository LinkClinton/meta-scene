#include "entity.hpp"

std::string metascene::entity::to_string() const noexcept
{
	std::stringstream stream;

	if (material != nullptr) stream << material->to_string() << std::endl;
	if (emitter != nullptr) stream << emitter->to_string() << std::endl;
	if (shape != nullptr) stream << shape->to_string() << std::endl;

	return stream.str();
}