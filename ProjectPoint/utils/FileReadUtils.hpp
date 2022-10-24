#pragma once

#include <fstream>
#include <sstream>
#include <string>

std::string readTextFromFile(const std::string& filePath)
{
	std::ifstream fileData;
	fileData.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		fileData.open(filePath);

		std::stringstream dataStream;
		dataStream << fileData.rdbuf();
		return dataStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		//TODO Add exception managment logic
		std::cout << e.what() << std::endl;
		return nullptr;
	}
}