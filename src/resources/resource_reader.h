#ifndef RESOURCEREADER_H
#define RESOURCEREADER_H

#include <vector>
#include <string>


class ResourceReader
{
public:
        static std::vector<char> readData(const char* relativePath);

private:
        ResourceReader() {};

        static const std::string ROOT_PATH;
};

#endif // RESOURCEREADER_H
