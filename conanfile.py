from conan import ConanFile


class BGLModelViewerRecipe(ConanFile):
    name = "bgl-3d-model-viewer"
    version = "1.0.0"
    package_type = "application"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("glew/2.2.0")
        self.requires("assimp/5.4.3")
        self.requires("glm/1.0.1")
