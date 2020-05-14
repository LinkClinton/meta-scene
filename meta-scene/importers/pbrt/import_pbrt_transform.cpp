#include "import_pbrt_transform.hpp"

#ifdef __PBRT_IMPORTER__

namespace metascene::importers::pbrt {

	void import_look_at(scene_context& context, matrix4x4& transform)
	{
		const auto origin = context.peek_vector3();
		const auto target = context.peek_vector3();
		const auto up = context.peek_vector3();

		transform *= look_at<real>(origin, target, up);
	}

	void import_rotate(scene_context& context, matrix4x4& transform)
	{
		const auto angle = radians(context.peek_real());
		const auto axis = context.peek_vector3();
			
		transform *= rotate<real>(axis, angle);
	}

}

#endif