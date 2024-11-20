#include <utils.h>
#include <cApp.h>

// Compile shader
unsigned int compileShader(GLenum type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        fprintf(stderr, "Shader Compilation Error: %s\n", infoLog);
    }
    return shader;
}

std::string loadFile(const std::string& filename) {

    cApp *app = cApp::getInstance();
    app->AddLogEntry("Loading file: " + filename, glm::vec3(1.0f, 1.0f, 0.0f));

    std::string content;
    std::ifstream file;
    file.open(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        file.close();
    } else {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    return content;
}

unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
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
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        fprintf(stderr, "Shader Linking Error: %s\n", infoLog);
    }

    // Delete shaders
    glDeleteShader(vs);
    glDeleteShader(fs);

    return shaderProgram;
}