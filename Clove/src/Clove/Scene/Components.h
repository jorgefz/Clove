#pragma once

#include "Clove/Renderer/Camera.h"
#include "glm/glm.hpp"

namespace Clove {

	struct TagComponent {
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& in_tag) : tag(in_tag) {  }
	};


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

	struct CameraComponent {
		Camera camera;
		bool primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& in_proj) : camera(in_proj) {  };
	};

}