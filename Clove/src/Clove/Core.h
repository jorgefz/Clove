#pragma once
#include "clovepch.h" // std headers

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


// Macros
#define CLOVE_ASSERT(condition, msg) if(!condition) { throw std::runtime_error(msg); }

#define BIND_METHOD_1(fn) std::bind(&fn, this, std::placeholders::_1)


