#include <utils.h>
#include <cApp.h>

// Compile shader
unsigned int compileShader(GLenum type, const char *source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        fprintf(stderr, "Shader Compilation Error: %s\n", infoLog);
    }
    return shader;
}

std::string loadFile(const std::string &filename)
{

    cApp *app = cApp::getInstance();
    app->AddLogEntry("Loading file: " + filename, glm::vec3(1.0f, 1.0f, 0.0f));

    std::string content;
    std::ifstream file;
    file.open(filename);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            content += line + "\n";
        }
        file.close();
    }
    else
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    return content;
}

unsigned int createShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int shaderProgram = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader.c_str());
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader.c_str());

    // Link shaders
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        fprintf(stderr, "Shader Linking Error: %s\n", infoLog);
    }

    // Delete shaders
    glDeleteShader(vs);
    glDeleteShader(fs);

    return shaderProgram;
}

#include <algorithm>
#include <cctype>
#include <locale>

std::string ltrim(std::string s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
    return s;
}

std::string rtrim(std::string s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
    return s;
}

// Trim from both ends (copying)
std::string trim(std::string s)
{
    return ltrim(rtrim(std::move(s)));
}

std::vector<std::string> split(const std::string &s, std::string delimiter)
{
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    std::string sCopy = s;
    while ((pos = sCopy.find(delimiter)) != std::string::npos)
    {
        token = sCopy.substr(0, pos);
        tokens.push_back(token);
        sCopy.erase(0, pos + delimiter.length());
    }
    tokens.push_back(sCopy);
    return tokens;
}