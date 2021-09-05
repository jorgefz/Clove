#pragma once

#include "Core.h"

namespace Clove {

	class Shader {
	private:
		std::string m_vshader_path, m_fshader_path;
		unsigned int m_renderer_id;
		std::unordered_map<std::string, int> m_location_cache;

	public:
		Shader(const std::string& vshader, const std::string& fshader);
		~Shader();
		void Bind() const;
		void Unbind() const;
		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	private:
		unsigned int Create();
		void ReadSource(std::string& filepath, std::string& source);
		unsigned int Compile(unsigned int type, const std::string& source);
		int GetUniformLocation(const std::string& name);
	};

}

