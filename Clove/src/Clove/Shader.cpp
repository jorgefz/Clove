#include "clovepch.h"
#include "Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"


namespace Clove {

	static GLenum ShaderTypeFromString(std::string& type) {
		if (type == "vertex") return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		std::cout << "Unknown shader type '" << type << "'" << std::endl;
		CLOVE_ASSERT(false, "");
		return 0;
	}

	static std::string ShaderNameFromPath(const std::string& filepath) {
		auto begin = filepath.find_last_of("/\\");
		begin = (begin == std::string::npos) ? 0 : begin + 1;
		auto end = filepath.rfind('.');
		if (end == std::string::npos) end = filepath.size() - 1;
		return filepath.substr(begin, end - begin);
	}


	Shader::Shader(const std::string& filepath) {
		m_name = ShaderNameFromPath(filepath);
		std::string source = Shader::ReadSource(filepath);
		auto shader_sources = Shader::Parse(source);
		Shader::Compile(shader_sources);
	}

	Shader::Shader(const std::string& vshader, const std::string& fshader)
		: m_renderer_id(0) {

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

	Ref<Shader> Shader::Create(const std::string& vshader, const std::string& fshader) {
		return std::make_shared<Shader>(vshader, fshader);
	}

	Ref<Shader> Shader::Create(const std::string& path) {
		return std::make_shared<Shader>(path);
	}

	void Shader::Bind() const {
		glUseProgram(m_renderer_id);
	}
	void Shader::Unbind() {
		glUseProgram(0);
	}

	const std::string& Shader::GetName() {
		return m_name;
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

	std::string Shader::ReadSource(const std::string& filepath) {

		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		std::string result;
		if (!in) {
			std::cout << "Error opening shader '" << filepath << "'" << std::endl;
			CLOVE_ASSERT(false, " ");
		}
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();

		return result;
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


	std::unordered_map<GLenum, std::string> Shader::Parse(const std::string& source) {
		
		std::unordered_map<GLenum, std::string> sources;
		const char* type_token = "#type";
		size_t token_len = strlen(type_token);
		size_t pos = source.find(type_token, 0);

		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			bool condition = eol != std::string::npos;

			CLOVE_ASSERT(condition, "Syntax error");
			size_t begin = pos + token_len + 1;
			std::string type = source.substr(begin, eol - begin);
			GLenum gl_type = ShaderTypeFromString(type);
			CLOVE_ASSERT(gl_type, "Unknown shader type");
			
			size_t next_line = source.find_first_not_of("\r\n", eol);
			pos = source.find(type_token, next_line);
			size_t offset = next_line == std::string::npos ? source.size() - 1 : next_line;
			sources[gl_type] = source.substr(next_line, pos - offset);
		}

		return sources;
	}

	void Shader::Compile(const std::unordered_map<GLenum, std::string>& sources) {
		unsigned int prog = glCreateProgram();
		CLOVE_ASSERT(sources.size() == 2, "Only fragment and vertex shaders supported");
		std::array<GLuint, 2> shader_ids;

		int count = 0;
		for (auto& kv : sources) {
			GLenum type = kv.first;
			const std::string& source = kv.second;
			
			GLuint shader = glCreateShader(type);
			const char* src = source.c_str();
			glShaderSource(shader, 1, &src, nullptr);
			glCompileShader(shader);

			// Was compilation sucessful?
			int result;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
			if (result != GL_TRUE) {
				// Retrieve compiler error message
				int length = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
				std::vector<GLchar> msg(length);
				glGetShaderInfoLog(shader, length, &length, &msg[0]);
				glDeleteShader(shader);
				std::cerr << "Failed to compile shader" << std::endl;
				std::cerr << msg.data() << std::endl;
				CLOVE_ASSERT(false, "");
			}

			glAttachShader(prog, shader);
			shader_ids[count++] = shader;
		}
		glLinkProgram(prog);
		glValidateProgram(prog);

		//Shader data has been copied to program and is no longer necessary.
		for (auto id : shader_ids) glDetachShader(prog, id);

		m_renderer_id = prog;
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
		int length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		std::vector<GLchar> msg(length);
		glGetShaderInfoLog(id, length, &length, &msg[0]);
		glDeleteShader(id);
		std::cerr << "Failed to compile shader" << std::endl;
		std::cerr << msg.data() << std::endl;
		CLOVE_ASSERT(false, "");
		return 0;
	}


	/* --- Shader Library --- */

	
	void ShaderLibrary::Add(const Ref<Shader>& shader) {
		auto& name = shader->GetName();
		CLOVE_ASSERT(!Contains(name), "Shader already in library");
		m_shaders[name] = shader;
	}


	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
		m_shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name) {
		CLOVE_ASSERT(Contains(name), "Shader not found");
		return m_shaders[name];
	}

	bool ShaderLibrary::Contains(const std::string& name) {
		return (m_shaders.find(name) != m_shaders.end());
	}

}
