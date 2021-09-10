#pragma once
#include "clovepch.h" // std headers

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


// Macros

#define BIND_METHOD_1(fn) std::bind(&fn, this, std::placeholders::_1)


