#ifndef RESOURCEREADER_H
#define RESOURCEREADER_H

#include <vector>
#include <string>


class AssetReader
{
public:
	static std::vector<char> readData(const char* relativePath);

private:
	AssetReader() = default;

	static const std::string ROOT_PATH;
};

#endif // RESOURCEREADER_H
