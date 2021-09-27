#pragma once

/*

* Core.h - variables & enums?
* Engine.h - old stuff
* Sprite.h - sprite class and methods
* Tilemap.h - tilemap class and methods
* UI.h - user interface class and methods
* Clove.h - includes everything

The client need only include "Clove.h"
and use the namespace "Clove::" to access classes and functions

*/

#include "Clove/Core.h"

#include "Clove/Window.h"
#include "Clove/Renderer/Camera.h"
#include "Clove/Renderer/Renderer.h"
#include "Clove/Renderer/RenderCommand.h"
#include "Clove/Renderer/VertexArray.h"
#include "Clove/Renderer/Buffer.h"
#include "Clove/Shader.h"
#include "Clove/Texture.h"

//#include "Clove/Tilemap.h"

#include "Clove/Events/Event.h"
#include "Clove/Events/KeyEvent.h"
#include "Clove/Events/MouseEvent.h"
#include "Clove/Events/WindowEvent.h"

#include "Clove/Input.h"
#include "Clove/KeyCodes.h"
#include "Clove/MouseCodes.h"

#include "Clove/GameApp.h"
#include "Clove/EntryPoint.h"

#include "Clove/Layer.h"
#include "Clove/ImGui/ImGuiLayer.h"