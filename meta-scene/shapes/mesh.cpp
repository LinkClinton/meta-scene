#include "mesh.hpp"

metascene::shapes::mesh::mesh() : shape(type::mesh)
{
}

metascene::shapes::mesh::mesh(const shapes::mesh_type& mesh_type, const std::string& filename) :
	shape(type::mesh), mesh_type(mesh_type), filename(filename)
{
}

std::string metascene::shapes::mesh::to_string() const noexcept
{
	std::stringstream stream;

	stream << "triangle_mesh : " << std::endl;
	stream << "filename = " << filename << std::endl;

	return stream.str();
}
