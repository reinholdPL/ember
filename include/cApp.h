#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <iomanip>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "cFrameBuffer.h"
#include "utils.h"
#include "cCamera.h"

typedef struct sLogEntry {
    std::string message;
    double time;
    glm::vec3 color;

    sLogEntry(std::string message, double time, glm::vec3 color) : message(message), time(time), color(color) {}

    void display()
    {
        //display timestamp, 5 zeroes before decimal point, 3 zeroes after
        std::cout << "[" << std::fixed << std::setprecision(5) << time << "] " << message << std::endl;
    }

} sLogEntry;

class cApp
{
private:
    std::vector<sLogEntry> logEntries;
    GLFWwindow *window;
    cFrameBuffer *sceneBuffer;

    //test
    unsigned int VBO, VAO, EBO;
    unsigned int shaderProgram, gridProgram;
    unsigned int gridVAO, gridVBO;
    std::vector<float> gridVertices;

    cCamera sceneCamera;
    
public:
    //this is singleton
    cApp() {}
    cApp(const cApp&) {  }
    cApp& operator=(const cApp&) { return *this; }

    static cApp* instance;

    static cApp* getInstance()
    {
        if (instance == nullptr)
            instance = new cApp();
        return instance;
    }

    //imgui stuff
    void ImguiDrawSceneFrame();
    void ImguiCheckIfMouseIsOver();

    // -----------

    void Init();
    int InitGLFW();
    int InitIMGUI();
    int InitOpenGL();
    void CenterWindow();
    void MaximizeWindow();
    void redrawScene();
    void AddLogEntry(std::string message, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
    std::vector<sLogEntry>* getLogEntries() { return &logEntries; }

    //setters and getters
    GLFWwindow* getWindow() { return window; }
    cFrameBuffer* getSceneBuffer() { return sceneBuffer; }
};