#pragma once
#include "clovepch.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Clove {

	class Shader {
	public:
		Shader(const std::string& vshader, const std::string& fshader);
		~Shader();
		static Shader* Create(const std::string& vshader, const std::string& fshader);
		
		void Bind() const;
		static void Unbind();

		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform2f(const std::string& name, glm::vec2& v);
		void SetUniform3f(const std::string& name, glm::vec3& v);
		void SetUniform4f(const std::string& name, glm::vec4& v);
		void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	private:
		int GetUniformLocation(const std::string& name);
		static void ReadSource(const std::string& filepath, std::string& source);
		static unsigned int Compile(unsigned int type, const std::string& source);
		
	private:
		std::string m_vshader_path, m_fshader_path;
		unsigned int m_renderer_id;
		std::unordered_map<std::string, int> m_location_cache;
	};

}

