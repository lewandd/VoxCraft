# VoxCraft

VoxCraft is a sandbox 3D game with procedurally generated world made of voxels. The aim of the project was to create Minecraft-like game optimized in context of RAM usage. For this purpose, low-level tools (C++, OpenGL) and various optimizing solutions were used.

## Getting started

### Prerequisites
Visual Studio environment and configuration_files.zip (for OpenGL API configuration) are required.

`configuration_files` directory includes:
- `Include` directory
- `Libs` directory
- `glad.c` single library file

### OpenGL Configuration in Visual Studio

1. Unpack configuration_files.zip
2. Open Visual Studio VoxCraft project
3. Set configuration_files paths
   - Project > Properties
   - Directories VC++ > Header File Directories > add path\
     	&emsp;../configuration_files/Include
   - Directories VC++ > Library Directories > add path\
	&emsp;.../configuration_files/Libs
5. Add required files to project
   - Project > Properties
   - Consolidator > Input > Additional Dependencies > add file \
     	&emsp;opengl32.lib glfw3.lib
   - Copy glad.c to project directory
   - Add glad.c to project
     - Right mouse button on project name
     - Add > Existing element... > glad.c
