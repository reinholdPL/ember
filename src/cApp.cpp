#include "cApp.h"
#include <filesystem>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

cApp *cApp::instance = nullptr;

cApp::cApp()
{
    sceneCamera.getTransform().setPosition(glm::vec3(-3.0f, 5.0f, 3.0f));
    sceneCamera.getTransform().setRotation(glm::vec3(-45.0f, -45.0f, 0.0f));
}

cApp::cApp(const cApp &)
{
    sceneCamera.getTransform().setPosition(glm::vec3(-3.0f, 5.0f, 3.0f));
    sceneCamera.getTransform().setRotation(glm::vec3(-45.0f, -45.0f, 0.0f));
}

std::string windowTypeToString(WindowType type)
{
    switch (type)
    {
    case NONE:
        return "None";
    case MAIN:
        return "Main";
    case ASSETS:
        return "Assets";
    case LOG:
        return "Log";
    case SCENE_EDITOR:
        return "Scene Editor";
    case OBJECT_PROPERTIES:
        return "Object Properties";
    default:
        return "Unknown";
    }
};

std::string mouseButtonToString(MouseButton button)
{
    switch (button)
    {
    case LEFT:
        return "Left";
    case RIGHT:
        return "Right";
    case MIDDLE:
        return "Middle";
    default:
        return "Unknown";
    }
}

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void cApp::Init()
{
    AddLogEntry("Application started", glm::vec3(0.0f, 1.0f, 0.0f));

    // std::cout << "Current path is " << std::filesystem::current_path() << '\n';
    // change to upper
    std::filesystem::current_path(std::filesystem::current_path().parent_path());

    InitGLFW();
    InitIMGUI();
    InitOpenGL();

    mesh.loadObj("assets/meshes/barrel.obj");
    mesh.genBuffers();

    // exit(1);
}

void cApp::CenterWindow()
{
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (!monitor)
    {
        return;
    }

    // Get monitor workarea
    int monitorX, monitorY;
    glfwGetMonitorPos(monitor, &monitorX, &monitorY);
    const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
    if (!vidmode)
    {
        return;
    }

    int windowWidth, windowHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &windowWidth, &windowHeight);
    glfwSetWindowPos(glfwGetCurrentContext(), monitorX + (vidmode->width - windowWidth) / 2, monitorY + (vidmode->height - windowHeight) / 2);
}

void cApp::MaximizeWindow()
{
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (!monitor)
    {
        return;
    }

    const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
    if (!vidmode)
    {
        return;
    }

    // glfwSetWindowSize(glfwGetCurrentContext(), vidmode->width, vidmode->height);
    glfwMaximizeWindow(glfwGetCurrentContext());
}

void cApp::AddLogEntry(std::string message, glm::vec3 color)
{
    _logFrameShouldBeScrolled = true;
    logEntries.push_back(sLogEntry(message, glfwGetTime(), color));
    // display in console
    logEntries.back().display();
}

int cApp::InitGLFW()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    // const char *glsl_version = "#version 440";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only


    window = glfwCreateWindow(1300, 1000, "ember3", nullptr, nullptr);
    if (window == nullptr)
        return 1;

    glfwSwapInterval(1); // Enable vsync
    

    glfwMakeContextCurrent(window);

    // CenterWindow();
    MaximizeWindow();

    return 0;
}

int cApp::InitIMGUI()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

    ImVector<ImWchar> ranges;
    ImFontGlyphRangesBuilder builder;
    builder.AddText("zażółć gęślą jaźń");                 // Add a string (here "Hello world" contains 7 unique characters)
    builder.AddChar(0x20B9);                              // Add a specific character
    builder.AddRanges(io.Fonts->GetGlyphRangesDefault()); // Add one of the default ranges
    builder.BuildRanges(&ranges);                         // Build the final result (ordered ranges with all the unique characters submitted)

    // load ./assets/fonts/FiraCodeNerdFontMono-Medium.ttf font
    io.Fonts->AddFontFromFileTTF("assets/fonts/FiraCodeNerdFontMono-Medium.ttf", 20.0f, NULL, ranges.Data);

    io.Fonts->Build();

    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 440");

    return 0;
}

int cApp::InitOpenGL()
{
    if (!gladLoadGL(glfwGetProcAddress))
    {
        AddLogEntry("Failed to initialize GLAD", glm::vec3(1.0f, 0.0f, 0.0f));
        return 1;
    }

    AddLogEntry("OpenGL Version: " + std::string((char *)glGetString(GL_VERSION)), glm::vec3(0.0f, 1.0f, 0.0f));

    // loading shaders
    std::string fragmentShaderSource = loadFile("assets/shaders/default.frag");
    std::string vertexShaderSource = loadFile("assets/shaders/default.vert");

    // Define the cube's vertices and indices
    float vertices[] = {
        // Positions          // Colors
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Bottom-left-back
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // Bottom-right-back
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,   // Top-right-back
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  // Top-left-back
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,  // Bottom-left-front
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // Bottom-right-front
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,    // Top-right-front
        -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f    // Top-left-front
    };

    unsigned int indices[] = {
        // Back face
        0, 1, 2,
        2, 3, 0,
        // Front face
        4, 5, 6,
        6, 7, 4,
        // Left face
        0, 4, 7,
        7, 3, 0,
        // Right face
        1, 5, 6,
        6, 2, 1,
        // Bottom face
        0, 1, 5,
        5, 4, 0,
        // Top face
        3, 2, 6,
        6, 7, 3};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Compile shaders
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

    // Create shader program and link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Program Linking Error: %s\n", infoLog);
    }

    // Delete shaders (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    sceneBuffer = new cFrameBuffer(1000, 1000);

    //

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0); // Positions
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float))); // Colors
    glEnableVertexAttribArray(1);

    // Unbind VAO (optional)
    glBindVertexArray(0);

    // creating grid
    // load shaders
    std::string gridFragmentShaderSource = loadFile("assets/shaders/grid.frag");
    std::string gridVertexShaderSource = loadFile("assets/shaders/grid.vert");

    // Compile shaders
    gridProgram = createShader(gridVertexShaderSource, gridFragmentShaderSource);

    float gridWidth = 20.0f;
    int divisions = 20;
    float step = gridWidth / divisions; // Grid step size

    // Horizontal lines
    for (int i = 0; i <= divisions; ++i)
    {
        float pos = -gridWidth / 2 + i * step;
        gridVertices.push_back(-gridWidth / 2);
        gridVertices.push_back(pos); // Start point
        gridVertices.push_back(gridWidth / 2);
        gridVertices.push_back(pos); // End point
    }

    // Vertical lines
    for (int i = 0; i <= divisions; ++i)
    {
        float pos = -gridWidth / 2 + i * step;
        gridVertices.push_back(pos);
        gridVertices.push_back(-gridWidth / 2); // Start point
        gridVertices.push_back(pos);
        gridVertices.push_back(gridWidth / 2); // End point
    }

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

    // Specify layout (position only)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return 0;
}

void cApp::redrawScene()
{

    // create sceneBuffer

    glViewport(0, 0, sceneBuffer->getSize().x, sceneBuffer->getSize().y);

    sceneBuffer->Bind();

    glEnable(GL_DEPTH_TEST);

    // Clear the color buffer
    glClearColor(0.45f, 0.55f, 0.60f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sceneCamera.setAspectRatio(sceneBuffer->getAspectRatio());
    sceneCamera.calculateProjectionMatrix();
    sceneCamera.calculateViewMatrix();

    // Draw grid
    glUseProgram(gridProgram);
    GLint projectionLoc = glGetUniformLocation(gridProgram, "projection");
    GLint viewLoc = glGetUniformLocation(gridProgram, "view");

    if (projectionLoc == -1)
        std::cerr << "Could not find projection uniform location!" << std::endl;

    if (viewLoc == -1)
        std::cerr << "Could not find view uniform location!" << std::endl;

    // Set the projection uniform value (assuming you have a glm::mat4 projection matrix)

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(sceneCamera.getProjectionMatrix()));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(sceneCamera.getViewMatrix()));

    // glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    // glEnable(GL_LINE_SMOOTH);

    // disable z test
    //  glDepthMask(GL_FALSE);
    glBindVertexArray(gridVAO);
    glDrawArrays(GL_LINES, 0, gridVertices.size() / 2);
    glBindVertexArray(0);

    // enable z test
    //  glDepthMask(GL_TRUE);

    glUseProgram(shaderProgram);

    // get projection uniform
    projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    viewLoc = glGetUniformLocation(shaderProgram, "view");

    if (projectionLoc == -1)
        std::cerr << "Could not find projection uniform location!" << std::endl;

    if (viewLoc == -1)
        std::cerr << "Could not find view uniform location!" << std::endl;

    // Set the projection uniform value (assuming you have a glm::mat4 projection matrix)

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(sceneCamera.getProjectionMatrix()));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(sceneCamera.getViewMatrix()));

    glBindVertexArray(mesh.getVAO());
    // glDrawElements(GL_TRIANGLES, mesh.getFacesCount()*3, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, mesh.getFacesCount() * 3);

    sceneBuffer->Unbind();
}

void cApp::ImguiDrawSceneFrame()
{

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("Scene");
    {

        // remove margins

        ImGui::BeginChild("GameRender");
        if (ImGui::IsWindowHovered())
        {
            _hoveredWindow = WindowType::SCENE_EDITOR;
        }

        float width = ImGui::GetContentRegionAvail().x;
        float height = ImGui::GetContentRegionAvail().y;
        glm::vec2 size = getSceneBuffer()->getSize();

        // check if the size has changed
        if (size.x != width || size.y != height)
        {
            getSceneBuffer()->RescaleFrameBuffer(width, height);
            redrawScene();
        }

        // *m_width = width;
        // *m_height = height;
        ImGui::Image(
            (ImTextureID)getSceneBuffer()->getFrameTexture(),
            ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::EndChild();
    }

    ImGui::End();
    ImGui::PopStyleVar();
}

void cApp::ImguiDrawSceneHierarchyFrame()
{
    ImGui::Begin("Scene hierarchy");

    // hovered window text

    ImGui::Text("Mouse is over content: %s", windowTypeToString(_hoveredWindow).c_str());
    ImGui::Text("%d", _hoveredWindow);

    ImGui::End();
}

void cApp::handleMouse()
{

    // get this frame mouse position
    double xpos, ypos;
    glm::vec2 mousePosition, mouseDelta;
    glfwGetCursorPos(window, &xpos, &ypos);
    mousePosition = glm::vec2(xpos, ypos);
    if (lastMousePosition.x == -1)
    {
        lastMousePosition = mousePosition;
        return; // this was first frame, so we don't care
    }

    mouseDelta = mousePosition - lastMousePosition;

    lastMousePosition = mousePosition;

    // ------ mouse buttons
    for (unsigned int btn = 0; btn < 3; btn++)
    {
        if (glfwGetMouseButton(window, btn) == GLFW_PRESS)
        {
            // check if bit 0 is set
            if ((lastMouseButtonState & (1 << btn)) == 0)
            {
                lastMouseButtonState |= (1 << btn);
                handeMouseButton((MouseButton)btn, true);
            }
        }
        else
        { // released
            if ((lastMouseButtonState & (1 << btn)))
            {
                lastMouseButtonState &= ~(1 << btn);
                handeMouseButton((MouseButton)btn, false);
            }
        }
    }

    // mouse movement

    if (mouseDelta.x != 0 || mouseDelta.y != 0)
    {
        handleMouseMovement(mouseDelta);
    }
}

void cApp::handeMouseButton(MouseButton button, bool pressed)
{
    AddLogEntry("Mouse button " + mouseButtonToString(button) + " " + (pressed ? "pressed" : "released"), glm::vec3(0.0f, 1.0f, 0.0f));

    if (button == _draggedMouseButton && _dragging)
    {
        if (!pressed)
        {
            _dragging = false;
            _draggedWindow = WindowType::NONE;
            _draggedMouseButton = MouseButton::BTN_NONE;
            AddLogEntry("drag end!", glm::vec3(0.0f, 1.0f, 0.0f));

            handleDragEnd();
        }
    }
}

void cApp::handleMouseMovement(glm::vec2 delta)
{
    if (lastMouseButtonState && !_dragging)
    { // any mouse button is pressed
        _draggedWindow = _hoveredWindow;
        _dragging = true;

        // get first pressed button
        for (unsigned int btn = 0; btn < 3; btn++)
        {
            if (lastMouseButtonState & (1 << btn))
            {
                _draggedMouseButton = (MouseButton)btn;
                break;
            }
        }

        handleDragStart(delta);

        AddLogEntry("drag start! button = " + mouseButtonToString(_draggedMouseButton), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    if (_dragging)
    {
        handleMouseDrag(delta);
    }
}

void cApp::handleMouseDrag(glm::vec2 delta)
{
    // AddLogEntry("dragging!", glm::vec3(0.0f, 1.0f, 0.0f));

    // if drag window is scene
    if (_draggedWindow == WindowType::SCENE_EDITOR && _draggedMouseButton == MouseButton::RIGHT)
    {
        glm::vec3 currentRotation = sceneCamera.getTransform().getRotation();
        currentRotation.x -= delta.y / 5.0f;
        currentRotation.x = glm::clamp(currentRotation.x, -90.0f, 90.0f);

        currentRotation.y -= delta.x / 5.0f;
        sceneCamera.getTransform().setRotation(currentRotation);
    }
}

void cApp::handleDragStart(glm::vec2 delta)
{
    // AddLogEntry("drag start!", glm::vec3(0.0f, 1.0f, 0.0f));

    // lock mouse cursor
    if (_draggedMouseButton == MouseButton::RIGHT)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void cApp::handleDragEnd()
{
    // AddLogEntry("drag end!", glm::vec3(0.0f, 1.0f, 0.0f));
    // unlock mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void cApp::handleKeyboard()
{
    // if dragging, right mouse button and window is scene
    if (_dragging && _draggedMouseButton == MouseButton::RIGHT && _draggedWindow == WindowType::SCENE_EDITOR)
    {
        // move camera left according to camera orientation
        glm::vec3 position = sceneCamera.getTransform().getPosition();
        glm::vec3 rotation = sceneCamera.getTransform().getRotation();

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            position.x -= _sceneMoveSpeed * sin(glm::radians(rotation.y + 90));
            position.z -= _sceneMoveSpeed * cos(glm::radians(rotation.y + 90));
            sceneCamera.getTransform().setPosition(position);
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            position.x += _sceneMoveSpeed * sin(glm::radians(rotation.y + 90));
            position.z += _sceneMoveSpeed * cos(glm::radians(rotation.y + 90));
            sceneCamera.getTransform().setPosition(position);
        }

        // on press W move FORWARD in direction of camera
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            position.x -= _sceneMoveSpeed * sin(glm::radians(rotation.y));
            position.z -= _sceneMoveSpeed * cos(glm::radians(rotation.y));
            position.y += _sceneMoveSpeed * sin(glm::radians(rotation.x));
            sceneCamera.getTransform().setPosition(position);
        }

        // on press S move BACKWARD in direction of camera
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            position.x += _sceneMoveSpeed * sin(glm::radians(rotation.y));
            position.z += _sceneMoveSpeed * cos(glm::radians(rotation.y));
            position.y -= _sceneMoveSpeed * sin(glm::radians(rotation.x));
            sceneCamera.getTransform().setPosition(position);
        }

        // key  q - move up camera , e - move down camera
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            position.y += _sceneMoveSpeed;
            sceneCamera.getTransform().setPosition(position);
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            position.y -= _sceneMoveSpeed;
            sceneCamera.getTransform().setPosition(position);
        }
    }
}

void cApp::ImguiDrawObjectProperties()
{

    ImGui::Begin("Object properties");
    {
        // ImGui::Text("Mouse is over content: %s", ImGui::IsWindowHovered() ? "true" : "false");

        // print camera position and rotation
        ImGui::Text("Camera position: %f %f %f", sceneCamera.getTransform().getPosition().x, sceneCamera.getTransform().getPosition().y, sceneCamera.getTransform().getPosition().z);
        ImGui::Text("Camera rotation: %f %f %f", sceneCamera.getTransform().getRotation().x, sceneCamera.getTransform().getRotation().y, sceneCamera.getTransform().getRotation().z);

        //fps
        ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
    }
}