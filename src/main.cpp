#include "utils.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <string>
#include <fstream>
#include <iostream>

#include <cApp.h>

std::string vertexShaderSource;
std::string fragmentShaderSource;

void RenderDockSpace()
{
	// Create a DockSpace
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// Get the current window's viewport
	ImGuiViewport *viewport = ImGui::GetMainViewport();

	// Set up a dockspace covering the entire main window
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("DockSpace Demo", nullptr, window_flags);

	// add to menubar
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(u8"Zażółć"))
		{
			if (ImGui::MenuItem(u8"gęślą"))
			{
			}
			if (ImGui::MenuItem("Open"))
			{
			}
			if (ImGui::MenuItem("Save"))
			{
			}
			if (ImGui::MenuItem("Exit"))
			{
				// glfwSetWindowShouldClose(app->getWindow(), 1);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo"))
			{
			}
			if (ImGui::MenuItem("Redo"))
			{
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::PopStyleVar(4);

	// DockSpace
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	ImGui::End();
}

// Main code
int main(int, char **)
{
	cApp *app = cApp::getInstance();
	app->Init();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!glfwWindowShouldClose(app->getWindow()))
	{
		app->handleMouse();
		app->handleKeyboard();

		app->setHoveredWindow(WindowType::NONE);
		app->redrawScene();

		glfwPollEvents();

		// get mouse position

		if (glfwGetWindowAttrib(app->getWindow(), GLFW_ICONIFIED) != 0)
		{
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		RenderDockSpace();

		{
			app->ImguiDrawSceneFrame();

			ImGui::Begin("Assets Manager");
			ImGui::End();

			ImGui::Begin("Log");

			// scroll this to bottom

			// write last 10 log entries
			std::vector<sLogEntry> *logEntries = app->getLogEntries();
			// write all log entries
			for (unsigned long i = 0; i < logEntries->size(); i++)
			{
				ImGui::TextColored(ImVec4((*logEntries)[i].color.r, (*logEntries)[i].color.g, (*logEntries)[i].color.b, 1.0f), "[%f] %s", (*logEntries)[i].time, (*logEntries)[i].message.c_str());
			}

			app->getLogFrameShouldBeScrolled()?ImGui::SetScrollHereY(1.0f):void(0);

			if (app->getLogFrameShouldBeScrolled()) {
				ImGui::SetScrollHereY(1.0f);
				app->setLogFrameShouldBeScrolled(false);
			}

			ImGui::End();

			app->ImguiDrawSceneHierarchyFrame();

			app->ImguiDrawObjectProperties();

			ImGui::End();

			// ImGui::Begin("Testing");
			// {
			// 	ImGui::Text("Hello, world!");
			// 	if (ImGui::Button("Press me!"))
			// 		app->AddLogEntry("Button pressed", glm::vec3(0.0f, 1.0f, 0.0f));
			// }
			// ImGui::End();
		}

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(app->getWindow(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		// // Use shader program and draw triangle
		// glUseProgram(shaderProgram);
		// glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 3);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(app->getWindow());

		// INPUT
		if (glfwGetKey(app->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(app->getWindow(), 1);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(app->getWindow());
	glfwTerminate();

	return 0;
}
