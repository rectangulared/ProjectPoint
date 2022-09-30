#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::string& vertShaderSource, const std::string& fragShaderSource)
{
	GLuint vertShaderID{ glCreateShader(GL_VERTEX_SHADER) }; 
	const GLchar* tempVertSource{ vertShaderSource.c_str() };
	glShaderSource(vertShaderID, 1, &tempVertSource, NULL);
	glCompileShader(vertShaderID);
	//TODO: Add shader compile error check

	GLuint fragShaderID{ glCreateShader(GL_FRAGMENT_SHADER) };
	const GLchar* tempFragSource{ fragShaderSource.c_str() };
	glShaderSource(fragShaderID, 1, &tempFragSource, NULL);
	glCompileShader(fragShaderID);
	//TODO: Add shader compile error check

	programID = glCreateProgram();
	glAttachShader(programID, vertShaderID);
	glAttachShader(programID, fragShaderID);
	glLinkProgram(programID);
	//TODO: Add shader program linking error

	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);
}

void ShaderProgram::use()
{
	glUseProgram(programID);
}
void ShaderProgram::deleteProgram()
{
	glDeleteProgram(programID);
}

void ShaderProgram::setBool(const std::string& name, const GLboolean& value)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::setInt(const std::string& name, const GLint& value)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::setUInt(const std::string& name, const GLuint& value)
{
	glUniform1ui(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, const GLfloat& value)
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::setVec2f(const std::string& name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec3f(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec4f(const std::string& name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setMat3f(const std::string& name, const glm::mat3& value)
{
	glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setMat4f(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

GLuint ShaderProgram::getProgramID()
{
	return programID;
}