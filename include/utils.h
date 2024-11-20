#include <string>
#include <fstream>
#include <glad/gl.h>
#include <cstdio>

std::string loadFile(const std::string& filename);
unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);