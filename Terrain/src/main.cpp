#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<random>
#include "render/shader.h"
#include "scene/camera.h"
#include"stb_image.h"
#include <iostream>
#include "GUI.h"
#include"scene/Light.h"
#include"scene/scene.h"
#include"SampleTerrainPass.h"
// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
GLfloat lerp(GLfloat a, GLfloat b, GLfloat f)
{
    return a + f * (b - a);
}
int main()
{

    GUI::Init();

    if (GUI::Create(SCR_WIDTH, SCR_HEIGHT) != 1) {
        glfwTerminate();
        return -1;
    }


    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    ImGui_ImplGlfwGL3_Init(GUI::window, true);

    // Setup style
    ImGui::StyleColorsDark();

    std::string s = "aaa";

    std::shared_ptr<Scene> scene = std::make_shared<Scene>(Camera(glm::vec3(0, 0, 0)), Light(glm::vec3(0, -0.304485, 2.43392), glm::vec3(1.0)), 100000.0f);
    RenderManager::get_instance().regisiterScene(scene);
    //scene->farPlane = 30.0f;
    //RenderManager::get_instance().regisiterRenderPass(std::make_shared<SampleTerrainPass>("SampleTerrainPass", "./shader/gpuheight.vs", "./shader/gpuheight.fs", "./shader/gpuheight.tcs", "./shader/gpuheight.tes"));
    RenderManager::get_instance().regisiterRenderPass(std::make_shared<SampleTerrainPass>("SampleTerrainPass", "./shader/gpuheight.vs", "./shader/gpuheight.fs"));
    RenderManager::get_instance().init();

    ProcessInpute::camera = scene->camera;


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(GUI::window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        ProcessInpute::deltaTime = currentFrame - ProcessInpute::lastFrame;
        ProcessInpute::lastFrame = currentFrame;

        // input
        // -----
        ProcessInpute::processInput(GUI::window);
        // render
        // ------

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        RenderManager::get_instance().update();

        GUI::RenderMainImGui();



        GUI::Update();


    }

    glfwTerminate();
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    return 0;
}


