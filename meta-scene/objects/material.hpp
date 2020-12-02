#pragma once

#include "texture.hpp"

namespace meta_scene::objects {
	struct material;
}

namespace meta_scene::objects::internal {

	struct diffuse_material {
		texture reflectance;
		texture sigma;
	};

	struct glass_material final {
		texture transmission;
		texture reflectance;

		texture roughness_u;
		texture roughness_v;
		texture eta;

		bool remapping = true;
	};

	struct metal_material final {
		texture roughness_u;
		texture roughness_v;
		
		texture eta;
		texture k;

		bool remapping = true;
	};

	struct mirror_material final {
		texture reflectance;
	};

	struct mixture_material final {
		std::shared_ptr<material> material0;
		std::shared_ptr<material> material1;

		texture alpha;
	};

	struct plastic_material final {
		texture specular;
		texture diffuse;

		texture roughness;
		texture eta;

		bool remapping = true;
	};

	struct substrate_material final {
		texture specular;
		texture diffuse;

		texture roughness_u;
		texture roughness_v;

		bool remapping = true;
	};

	struct subsurface_material final {
		texture transmission;
		texture reflectance;
		texture diffuse;
		texture mfp;

		texture roughness_u;
		texture roughness_v;
		texture eta;

		bool remapping = true;
	};

	struct translucent_material final {
		texture transmission;
		texture reflectance;
		texture specular;
		texture diffuse;

		texture roughness;

		bool remapping = true;
	};

	struct uber_material final {
		texture transmission;
		texture reflectance;
		texture specular;
		texture diffuse;

		texture opacity;

		texture roughness_u;
		texture roughness_v;
		texture eta;

		bool remapping = true;
	};
}

namespace meta_scene::objects {

	struct material final {
		internal::translucent_material translucent;
		internal::subsurface_material subsurface;
		internal::substrate_material substrate;
		internal::mixture_material mixture;
		internal::diffuse_material diffuse;
		internal::plastic_material plastic;
		internal::mirror_material mirror;
		internal::glass_material glass;
		internal::metal_material metal;
		internal::uber_material uber;
		
		std::string type = "unknown";
	};
	
}
