#include "clovepch.h"
#include "Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"


namespace Clove {
	Shader::Shader(const std::string& vshader, const std::string& fshader)
		: m_vshader_path(vshader), m_fshader_path(fshader), m_renderer_id(0) {
		
		std::string vertex_source, fragment_source;

		//Read shader scripts
		Shader::ReadSource(vshader, vertex_source);
		Shader::ReadSource(fshader, fragment_source);
		m_renderer_id = glCreateProgram();
		GLuint vs = Shader::Compile(GL_VERTEX_SHADER, vertex_source);
		GLuint fs = Shader::Compile(GL_FRAGMENT_SHADER, fragment_source);

		glAttachShader(m_renderer_id, vs);
		glAttachShader(m_renderer_id, fs);
		glLinkProgram(m_renderer_id);
		glValidateProgram(m_renderer_id);

		//Shader data has been copied to program and is no longer necessary.
		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	Shader::~Shader() {
		Shader::Unbind();
		glDeleteProgram(m_renderer_id);
	}

	Shader* Shader::Create(const std::string& vshader, const std::string& fshader) {
		return new Shader(vshader, fshader);
	}

	void Shader::Bind() const {
		glUseProgram(m_renderer_id);
	}
	void Shader::Unbind() {
		glUseProgram(0);
	}
	void Shader::SetUniform1i(const std::string& name, int value) {
		glUniform1i(GetUniformLocation(name), value);
	}
	void Shader::SetUniform1f(const std::string& name, float value) {
		glUniform1f(GetUniformLocation(name), value);
	}
	void Shader::SetUniform2f(const std::string& name, glm::vec2& v) {
		glUniform2f(GetUniformLocation(name), v.x, v.y);
	}
	void Shader::SetUniform3f(const std::string& name, glm::vec3& v) {
		glUniform3f(GetUniformLocation(name), v.x, v.y, v.z);
	}
	void Shader::SetUniform4f(const std::string& name, glm::vec4& v) {
		glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w);
	}
	void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix) {
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}
	void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	/* Private methods */

	int Shader::GetUniformLocation(const std::string& name) {
		if (m_location_cache.find(name) != m_location_cache.end()) return m_location_cache[name];
		int location = glGetUniformLocation(m_renderer_id, name.c_str());
		if (location == -1) std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
		m_location_cache[name] = location;
		return location;
	}

	void Shader::ReadSource(const std::string& filepath, std::string& source) {
		std::ifstream stream(filepath);
		std::string line;
		std::stringstream ss;
		if (stream.fail()) {
			std::cerr << "Error: shader file '" << filepath << "' not found" << std::endl;
			throw std::runtime_error("");
		}
		while (std::getline(stream, line)) ss << line << '\n';
		source = ss.str();
	}

	unsigned int Shader::Compile(unsigned int type, const std::string& source) {
		GLuint id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		// Was compilation sucessful?
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_TRUE) return id;
		
		// Retrieve compiler error message
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length + 1];
		glGetShaderInfoLog(id, length, &length, message);
		message[length] = '\0';
		
		std::cerr << "[GL] Error: failed to compile shader!" << std::endl;
		std::cerr << message << std::endl;
		
		glDeleteShader(id);
		delete[] message;
		exit(-1);
	}
}
