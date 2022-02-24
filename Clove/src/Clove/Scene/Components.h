#pragma once

#include "glm/glm.hpp"

namespace Clove {

	struct TransformComponent {
		glm::mat4 transform = glm::mat4{1.0f};

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& in_transform) : transform(in_transform) {  };

		// this is shady, allows to treat this struct like a glm::vec4 when passing to functions
		operator glm::mat4& () { return transform; }
		operator const glm::mat4& () const { return transform; }
	};

	struct SpriteRendererComponent {

		glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& in_color) : color(in_color) {  };

	};


}