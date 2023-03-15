#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {};

ShaderProgram::ShaderProgram(const std::string& vertShaderSource, const std::string& fragShaderSource)
{
	GLuint vertShaderID{ glCreateShader(GL_VERTEX_SHADER) }; 
	const GLchar* tempVertSource{ vertShaderSource.c_str() };
	glShaderSource(vertShaderID, 1, &tempVertSource, NULL);
	glCompileShader(vertShaderID);
	GLint isCompiled = 0;
	glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &maxLength);

		char errorLog[256];
		glGetShaderInfoLog(vertShaderID, maxLength, &maxLength, &errorLog[0]);

		std::cout << errorLog << std::endl;

		glDeleteShader(vertShaderID);
		return;
	}

	GLuint fragShaderID{ glCreateShader(GL_FRAGMENT_SHADER) };
	const GLchar* tempFragSource{ fragShaderSource.c_str() };
	glShaderSource(fragShaderID, 1, &tempFragSource, NULL);
	glCompileShader(fragShaderID);
	isCompiled = 0;
	glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &maxLength);

		char errorLog[256];
		glGetShaderInfoLog(fragShaderID, maxLength, &maxLength, &errorLog[0]);

		std::cout << errorLog << std::endl;

		glDeleteShader(fragShaderID);
		return;
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertShaderID);
	glAttachShader(programID, fragShaderID);
	glLinkProgram(programID);
	GLint isLinked = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char errorLog[256];
		glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(programID);
		// Don't leak shaders either.
		glDeleteShader(vertShaderID);
		glDeleteShader(fragShaderID);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave
		return;
	}

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