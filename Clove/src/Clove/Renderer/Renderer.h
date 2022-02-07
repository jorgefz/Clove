#pragma once
#include "Clove/Renderer/Buffer.h"
#include "Clove/Renderer/VertexArray.h"
#include "Clove/Renderer/Camera.h"
#include "Clove/Shader.h"

namespace Clove {
	class Renderer {
	
	public:
		static void Init();
		static void OnWindowResize(unsigned int width, unsigned int height);
		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Submit(
			const Ref<VertexArray>& va,
			const Ref<Shader>& shader,
			glm::mat4& transform = glm::mat4(1.0f)
		);

	private:
		struct SceneData {
			glm::mat4 vp; // view projection matrix
		};
		static SceneData* m_scene_data;
	};

}
