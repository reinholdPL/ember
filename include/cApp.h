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

#include "cMesh.h"

enum WindowType
{
    NONE,
    MAIN,
    ASSETS,
    LOG,
    SCENE_EDITOR,
    OBJECT_PROPERTIES
};

enum MouseButton
{
    LEFT = 0,
    RIGHT = 1,
    MIDDLE = 2,
    BTN_NONE = 3
};

std::string windowTypeToString(WindowType type);
std::string mouseButtonToString(MouseButton button);

typedef struct sLogEntry
{
    std::string message;
    double time;
    glm::vec3 color;

    sLogEntry(std::string message, double time, glm::vec3 color) : message(message), time(time), color(color) {}

    void display()
    {
        // display timestamp, 5 zeroes before decimal point, 3 zeroes after
        std::cout << "[" << std::fixed << std::setprecision(5) << time << "] " << message << std::endl;
    }

} sLogEntry;

class cApp
{
private:
    std::vector<sLogEntry> logEntries;
    GLFWwindow *window;
    cFrameBuffer *sceneBuffer;

    // test
    cMesh mesh;
    unsigned int VBO, VAO, EBO;
    unsigned int shaderProgram, gridProgram;
    unsigned int gridVAO, gridVBO;
    double _sceneMoveSpeed = 0.1;
    WindowType _hoveredWindow = WindowType::NONE;
    std::vector<float> gridVertices;
    bool _logFrameShouldBeScrolled = false;

    cCamera sceneCamera;
    glm::vec2 lastMousePosition = glm::vec3(-1.0f, 0.0f, 0.0f);
    char lastMouseButtonState = 0;
    bool _dragging = false;
    WindowType _draggedWindow = WindowType::NONE;
    MouseButton _draggedMouseButton = MouseButton::BTN_NONE;


public:
    // this is singleton
    cApp() ;
    cApp(const cApp &);
    cApp &operator=(const cApp &) { return *this; }

    static cApp *instance;

    static cApp *getInstance()
    {
        if (instance == nullptr)
            instance = new cApp();
        return instance;
    }

    // imgui stuff
    bool getLogFrameShouldBeScrolled() { return _logFrameShouldBeScrolled; }
    void setLogFrameShouldBeScrolled(bool value) { _logFrameShouldBeScrolled = value; }
    void ImguiDrawSceneFrame();
    void ImguiCheckIfMouseIsOver();
    void ImguiDrawSceneHierarchyFrame();
    void ImguiDrawObjectProperties();

    // -----------

    void Init();
    int InitGLFW();
    int InitIMGUI();
    int InitOpenGL();
    void CenterWindow();
    void MaximizeWindow();
    void redrawScene();
    void AddLogEntry(std::string message, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
    std::vector<sLogEntry> *getLogEntries() { return &logEntries; }

    // setters and getters
    GLFWwindow *getWindow() { return window; }
    cFrameBuffer *getSceneBuffer() { return sceneBuffer; }

    // do usunięcia po zaimplementowaniu
    void setHoveredWindow(WindowType window) { _hoveredWindow = window; }

    // input management
    void handleMouse();
    void handeMouseButton(MouseButton button, bool pressed);
    void handleMouseMovement(glm::vec2 delta);
    void handleMouseDrag(glm::vec2 delta);
    void handleDragStart(glm::vec2 delta);
    void handleDragEnd();

    void handleKeyboard();
};