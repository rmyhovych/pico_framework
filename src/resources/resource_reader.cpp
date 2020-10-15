#include "resource_reader.h"

#include <fstream>

#ifdef _WIN32
        #include <windows.h>
        #include <Shlwapi.h>
        #include <io.h>

        #define access _access_s
#endif // _WIN32

std::string readRootPath()
{
        std::string execPath;

#ifdef _WIN32
        execPath.resize(MAX_PATH);
        GetModuleFileNameA(NULL, (LPSTR) execPath.data(), MAX_PATH);
#endif // _WIN32

        std::string rootName("VulkanTest");
        size_t index = execPath.find(rootName);

        return execPath.substr(0, index + rootName.size() + 1);
}

const std::string ResourceReader::ROOT_PATH = readRootPath();


std::vector<char> ResourceReader::readData(const char *relativePath)
{
    std::string fullPath = ROOT_PATH + relativePath;
    std::ifstream file(fullPath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
            throw std::runtime_error("Failed to open file " + fullPath);
    }

    std::vector<char> fileContent(file.tellg());
    file.seekg(std::ios::beg);

    file.read(fileContent.data(), fileContent.size());

    file.close();

    return fileContent;
}
