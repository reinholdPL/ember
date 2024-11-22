#include <string>
#include <fstream>
#include <glad/gl.h>
#include <cstdio>
#include <vector>

std::string loadFile(const std::string& filename);
unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
std::string trim(std::string s);
std::string rtrim(std::string s);
std::string ltrim(std::string s);
std::vector<std::string> split(const std::string &s, std::string delimiter);