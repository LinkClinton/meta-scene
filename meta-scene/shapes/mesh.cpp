#include "mesh.hpp"

metascene::shapes::mesh::mesh() : shape(type::mesh)
{
}

metascene::shapes::mesh::mesh(const shapes::mesh_type& mesh_type, const std::string& filename) :
	mesh_type(mesh_type), filename(filename)
{
}
