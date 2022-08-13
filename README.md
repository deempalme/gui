# Ramrod's Graphic user interface with Vulkan

## Installation

### Installing Vulkan

```bash
# Command-line utilities, most importantly vulkaninfo and vkcube
sudo apt install vulkan-tools
# Installing vulkan's loader
# The loader looks up the functions in the driver at runtime, similarly to GLEW for OpenGL
sudo apt install libvulkan-dev
# Installs the standard validation layers and required SPIR-V tools.
# These are crucial when debugging Vulkan applications
sudo apt install vulkan-validationlayers-dev spirv-tools
```

### Installing a GLSL compiler

```bash
# To compile use: glslangValidator [--help]
sudo apt install glslang-dev glslang-tools
```