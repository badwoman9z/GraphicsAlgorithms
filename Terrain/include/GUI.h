#pragma once
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "scene/camera.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include"render/RenderManager.h"

namespace ProcessInpute {
    const unsigned int SCR_WIDTH = 1920;
    const unsigned int SCR_HEIGHT = 1080;
    bool firstMouse = true;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    std::shared_ptr<Camera> camera;
   


    void processInput(GLFWwindow* window)
    {


        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                camera->ProcessKeyboard(FORWARD, deltaTime);
            }

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                camera->ProcessKeyboard(BACKWARD, deltaTime);
            }

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera->ProcessKeyboard(LEFT, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera->ProcessKeyboard(RIGHT, deltaTime);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
    {

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {

            camera->ProcessMouseMovement(xoffset, yoffset);
           

        }

    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera->ProcessMouseScroll(static_cast<float>(yoffset));
    }

}
namespace GUI {
	GLFWwindow* window;
    bool show_demo_window = true;
    bool show_another_window = false;
    static float f = 0.0f;
    static int counter = 0;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	void Init() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	 }
    void RenderMainImGui()
    {
        
        ImGui_ImplGlfwGL3_NewFrame();

        {   
            glm::vec3 &dir = RenderManager::get_instance().getScene()->light.LightDir;
            glm::vec3 &pos = RenderManager::get_instance().getScene()->light.LightPos;

            glm::vec3 &cameraPosition = RenderManager::get_instance().getScene()->camera->Position;
            glm::vec3 &cameraFront = RenderManager::get_instance().getScene()->camera->Front;
            glm::vec3 &cameraUp = RenderManager::get_instance().getScene()->camera->Up;
            glm::vec3 &cameraRight = RenderManager::get_instance().getScene()->camera->Right;
            glm::vec3 &cameraWorldUp = RenderManager::get_instance().getScene()->camera->WorldUp;


            ImGui::SliderFloat3("cameraPosition", &cameraPosition.x, -50.0f, 50.0f);
            ImGui::SliderFloat3("cameraFront", &cameraFront.x, -50.0f, 50.0f);
            ImGui::SliderFloat3("cameraUp", &cameraUp.x, -50.0f, 50.0f);
            ImGui::SliderFloat3("cameraRight", &cameraRight.x, -50.0f, 50.0f);
            ImGui::SliderFloat3("cameraWorldUp", &cameraWorldUp.x, -50.0f, 50.0f);
                                   
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);   
            ImGui::SliderFloat3("dir", &dir.x, -10.0f, 10.0f); 
            ImGui::SliderFloat3("pos", &pos.x, -3.0f, 3.0f); // Edit 1 float using a slider from 0.0f to 1.0f    
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
  

        // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
        if (show_demo_window)
        {
            ImGui::SetNextWindowPos(ImVec2(800, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
            ImGui::ShowDemoWindow(&show_demo_window);
        }
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    }

	int Create(const unsigned int SCR_WIDTH , const unsigned int SCR_HEIGHT) {
		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			return -1;
		}

		glfwMakeContextCurrent(window);
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Error::Window:: GLEW Init Failure" << std::endl;

        }
		glfwSetFramebufferSizeCallback(window,ProcessInpute::framebuffer_size_callback);
		glfwSetCursorPosCallback(window, ProcessInpute::mouse_callback);
		glfwSetScrollCallback(window, ProcessInpute::scroll_callback);
		// tell GLFW to capture our mouse
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		return 1;
	}

	void Update() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}
