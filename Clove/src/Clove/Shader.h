#pragma once
#include "clovepch.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Clove {

	typedef unsigned int GLenum; // forward decl

	class Shader {
	public:
		// create shader from single file
		Shader(const std::string& filepath);
		// create shader from separate vertex and fragment files
		Shader(const std::string& vshader, const std::string& fshader);
		~Shader();
		static Shader* Create(const std::string& vshader, const std::string& fshader);
		static Shader* Shader::Create(const std::string& path);
		
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

		std::unordered_map<GLenum, std::string> Parse(const std::string& source);
		
		// binary mode
		static std::string ReadSource(const std::string& filepath);
		// char mode
		static void ReadSource(const std::string& filepath, std::string& source);

		// compiles list of shaders
		void Compile(const std::unordered_map<GLenum, std::string>& sources);
		// compiles single shader of given type
		static unsigned int Compile(unsigned int type, const std::string& source);
		

	private:
		std::string m_vshader_path, m_fshader_path;
		unsigned int m_renderer_id;
		std::unordered_map<std::string, int> m_location_cache;
	};

}

