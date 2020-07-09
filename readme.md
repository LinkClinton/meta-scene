# Meta-Scene

Meta-scene is a library to load different scene format. Using meta-scene to load scene file and create the scene to your renderer with `meta-scene` type.

## Build

- Clone or download the repository.
- Using `vcpkg` to install third-parties.
- Open `.sln` with Visual Studio 2019 and build it.

## Support Format

- Part of mitsuba scene format. 
- Most of PBRT scene format.

## Test

There are scenes passed the test. You can click [here](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes) to see the image of scene(render by rainbow-renderer).

- [cbox/cbox.xml](https://github.com/LinkClinton/repository-data/blob/master/rainbow-renderer/scenes/cbox) : cornell box.
- [cbox-sss/cbox.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/cbox-sss) : cornell box with bssrdf.
- [water-caustic/scene.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/water-caustic) : sppm integrator.
- [volumetric-caustic/scene.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/volumetric-caustic) : volume bdpt integrator.
- [veach_mi/mi.xml](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/veach_mi) : veach's MIS test scene.
- [veach-bidir/scene.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/veach-bidir) : veach's bdpt test scene.
- [bunny-fur/f3-15.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/bunny-fur) : bunny with fur, curve test scene.
- [dragon/f8-10.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/dragon) : dragon with plastic/glass material.
- [lte-orb/lte-orb-silver.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/lte-orb) : lte-orb with metal material.
- [structuresynth/metal.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/structuresynth) : metal material scene.
- [structuresynth/arcsphere.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/structuresynth) : glass material scene.
- [structuresynth/microcity.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/structuresynth) : lens test scene.
- [structuresynth/ballpile.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/structuresynth) : bssrdf test scene.
- [yeahright/yeahright.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/yeahright) : metal material scene.
- [pbrt-book/book.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/pbrt-book) : pbrt-v2 book scene(not support bump mapping).
- [ganesha/ganesha.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/ganesha) : substrate material scene.
- [chopper-titan/chopper-titan.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/chopper-titan) : chopper scene.
- [tt/tt.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/tt) : car scene(not support bump mapping).
- [measure-one/frame380.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/measure-one) : [ Beeple's amazing Zero-Day animation](https://www.beeple-crap.com/films)'s frame(from c4d to pbrt) scene. 
- [living-room-3/scene.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/living-room-3) : living scene.
- [kitchen/scene.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/kitchen) : kitchen scene.
- [buddha-fractal/buddha-fractal.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/buddha-fractal) : instance of bvh scene.
- [bathroom/bathroom.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/bathroom) : bathroom scene(replace fourier material to mixture material).
- [landscape/view-0.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/landscape) : landscape scene.
- [crown/crown.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/crown) : crown scene(not support bump mapping).
- [ecosys/ecosys.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/ecosys) : ecosys scene(not support bump mapping and windy texture).
- [bmw-m6/bmw-m6.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/bmw-m6) : bmw car scene(replace fourier material to matte material).
- [vw-van/vw-van.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/vw-van) : car scene(replace fourier material to matte material).
- [sssdragon/dragon_10.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/sssdragon) : dragon with bssrdf.
- [head/head.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/head) : head with bssrdf.
- [cloud/cloud.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/cloud) : cloud.
- [coffee-splash/splash.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/coffee-splash) : coffee((replace fourier material to mixture material).
- [dambreak/dambreak0.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/dambreak): water.
- [dragon/f11-14.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/dragon) : dragon with glass material and medium.
- [smoke-plume/plume-084.pbrt](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes/smoke-plume) : smoke with mirror sphere. 

All scenes you can download from the web of mitsuba, pbrt and [benedikt bitterli](https://benedikt-bitterli.me/resources/). But some scenes(pbrt format) are changed for rendering(for example, replace "bdpt" to "path").

You can click [here](https://github.com/LinkClinton/repository-data/tree/master/rainbow-renderer/scenes) to get the changed scenes(only include the scene file, no textures/meshes/spds). Or you can click the name of scene.

## ThirdParties

Different importer need different third-parties. You do not need install third-parties other importers used.

- Mitsuba importer : [tinyxml2](https://github.com/leethomason/tinyxml2).
- PBRT importer : null.
- [glm](https://github.com/g-truc/glm): OpenGL Mathematics (GLM) 