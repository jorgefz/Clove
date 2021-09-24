#pragma once
#include "Clove/Renderer/Buffer.h"
#include "Clove/Renderer/VertexArray.h"
#include "Clove/Renderer/Camera.h"
#include "Clove/Shader.h"

namespace Clove {
	class Renderer {
	
	public:
		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader);
	
	private:
		struct SceneData {
			glm::mat4 vp; // view projection matrix
		};
		static SceneData* m_scene_data;
	};

}
