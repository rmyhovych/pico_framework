#include "asset_reader.h"

#include <fstream>

#include <iostream>

std::vector<char> AssetReader::readData(const char* relativePath)
{
	std::ifstream file(relativePath, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file " + std::string(relativePath));
	}

	std::vector<char> fileContent(file.tellg());
	file.seekg(std::ios::beg);

	file.read(fileContent.data(), fileContent.size());

	file.close();

	return fileContent;
}
