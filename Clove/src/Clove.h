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


// Core
#include "Clove/Core/Core.h"
#include "Clove/Core/GameApp.h"
#include "Clove/Core/Window.h"
#include "Clove/Core/Input.h"
#include "Clove/Core/KeyCodes.h"
#include "Clove/Core/MouseCodes.h"
#include "Clove/Core/Layer.h"


// Renderer
#include "Clove/Renderer/Renderer.h"
#include "Clove/Renderer/Renderer2D.h"
#include "Clove/Renderer/RenderCommand.h"
#include "Clove/Renderer/VertexArray.h"
#include "Clove/Renderer/Buffer.h"
#include "Clove/Renderer/Shader.h"
#include "Clove/Renderer/Texture.h"
#include "Clove/Renderer/SubTexture2D.h"

#include "Clove/Renderer/Camera.h"
#include "Clove/Renderer/CameraController.h"

// Events
#include "Clove/Events/Event.h"
#include "Clove/Events/KeyEvent.h"
#include "Clove/Events/MouseEvent.h"
#include "Clove/Events/WindowEvent.h"

// ImGui
#include "Clove/ImGui/ImGuiLayer.h"

