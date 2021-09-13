# Clove
2D game engine written in C++ that uses OpenGL for rendering. Based on the Youtube tutorials by The Cherno (https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw)

## Build instructions

This project is built using Visual Studio 2019 and is targeted for Windows exclusively.

First, use Premake5 to build the VS2019 solution by running the "GenerateProjects.bat" file, or running the following command:
```
Clove\vendor\premake\bin\premake5.exe vs2019
```

Then, open the solution file Clove.sln, and build the solution. You should then find the game executable in 'bin/Config-Platform-Arch/Game/Game.exe'.

## Dependencies
This project uses public domain software, which is already included in this repository.

* std_image.h, by Sean Barret (https://github.com/nothings/stb/blob/master/stb\_image.h)
* GLFW, Graphics Library Framework (https://www.glfw.org/)
* Glad, https://glad.dav1d.de/
* glm, OpenGL Mathematics (https://github.com/g-truc/glm)
* Premake5 (https://premake.github.io/)
* ImGui (https://github.com/ocornut/imgui/releases/tag/v1.84.2 )