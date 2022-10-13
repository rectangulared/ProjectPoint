#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram
{
public:
	
	ShaderProgram();
	ShaderProgram(const std::string& vertShaderSource, const std::string& fragShaderSource);

	void use();
	void deleteProgram();

	void setBool(const std::string& name, const GLboolean& value);
	void setInt(const std::string& name, const GLint& value);
	void setUInt(const std::string& name, const GLuint& value);
	void setFloat(const std::string& name, const GLfloat& value);
	void setVec2f(const std::string& name, const glm::vec2& value);
	void setVec3f(const std::string& name, const glm::vec3& value);
	void setVec4f(const std::string& name, const glm::vec4& value);
	void setMat3f(const std::string& name, const glm::mat3& value);
	void setMat4f(const std::string& name, const glm::mat4& value);

	GLuint getProgramID();
private:
	GLuint programID;
};