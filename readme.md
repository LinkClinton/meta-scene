# Meta-Scene

Meta-scene is a library to load different scene format. Using meta-scene load scene file and create the scene to your renderer with `meta-scene` type.

## Build

- Clone or download the repository.
- Using `vcpkg` to install third-parties.
- Open `.sln` with Visual Studio 2019 and build it.

## Support Format

- Part of mitsuba scene format. 
- Part of PBRT scene format.

## ThirdParties

Different importer need different third-parties. You do not need install third-parties other importers used.

- Mitsuba importer : [tinyxml2](https://github.com/leethomason/tinyxml2).
- PBRT importer : null.
- [glm](https://github.com/g-truc/glm): OpenGL Mathematics (GLM) 