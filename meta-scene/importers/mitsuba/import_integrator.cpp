#include "import_integrator.hpp"

#include "../../integrators/path_integrator.hpp"

#ifdef __MITSUBA_IMPORTER__

namespace metascene::importers::mitsuba {


	void import_integrator(const tinyxml2::XMLNode* node, std::shared_ptr<integrator>& integrator)
	{
		integrator = std::make_shared<path_integrator>();
	}
}

#endif