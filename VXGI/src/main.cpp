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

#include"GbufferPass.h"
#include"VoxelizePass.h"
#include"VoxelvisualizePass.h"
#include"RSMBufferPass.h"
#include"ScreenQuadPass.h"
#include"FrontWorldPosPass.h"
#include"BackWorldPosPass.h"
#include"ConeTracingPass.h"


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
    for (int i = 0; i <= 4; i++) {
        glm::vec4 a(0, 0.866025, 0.5, 1.0);
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, glm::radians(72.0f * i), glm::vec3(0.0, 0.0, 1.0));
        a = trans * a;
        std::cout << a.x << " " << a.y << " " << a.z << std::endl;

    }


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

    std::shared_ptr<Scene> scene = std::make_shared<Scene>(Camera(glm::vec3(0, 0, 0)), Light(glm::vec3(0, -0.304485, 2.43392), glm::vec3(1.0)));
    //scene->LoadScene("d:/user/wkx/desktop/grapincs/rendertoy/gi/model/sponza.obj");

    scene->LoadModel("d:/user/wkx/desktop/grapincs/rendertoy/gi/model/sponza.obj");

    RenderManager::get_instance().regisiterScene(scene);



    RenderManager::get_instance().regisiterRenderPass(std::make_shared<RSMBufferPass>("RSMBufferPass", "./shader/RSMBufferPass.vs", "./shader/RSMBufferPass.fs"));

    //RenderManager::get_instance().regisiterRenderPass(std::make_shared<GbufferPass>("Gbuffer", "./shader/VXGI/gbuffer.vs", "./shader/VXGI/gbuffer.fs"));

    //RenderManager::get_instance().regisiterRenderPass(std::make_shared<ScreenQuadPass>("ScreenQuadPass", "./shader/VXGI/ScreenQuadPass.vs", "./shader/VXGI/ScreenQuadPass.fs"));

   // RenderManager::get_instance().regisiterRenderPass(std::make_shared<RenderLightPass>("RenderLightPass", "./shader/VXGI/RenderLightPass.vs", "./shader/VXGI/RenderLightPass.fs"));
    RenderManager::get_instance().regisiterRenderPass(std::make_shared<VoxelizePass>("VoxelizePass", "./shader/VoxelizePass.vs", "./shader/VoxelizePass.fs", "./shader/VoxelizePass.gs"));
    //RenderManager::get_instance().regisiterRenderPass(std::make_shared<VoxelLightInjectPass>("VoxelLightInjectPass", "./shader/VXGI/VoxelLightInjectPass.comp"));

   //RenderManager::get_instance().regisiterRenderPass(std::make_shared<FrontWorldPosPass>("FrontWorldPosPass", "./shader/VXGI/world_pos_vert.vs", "./shader/VXGI/world_pos_vert.fs"));
   //RenderManager::get_instance().regisiterRenderPass(std::make_shared<BackWorldPosPass>("BackWorldPosPass", "./shader/VXGI/world_pos_vert.vs", "./shader/VXGI/world_pos_vert.fs"));

    //RenderManager::get_instance().regisiterRenderPass(std::make_shared<VoxelvisualizePass>("VoxelvisualizePass", "./shader/VXGI/VoxelvisualizePass.vs", "./shader/VXGI/VoxelvisualizePass.fs"));

     RenderManager::get_instance().regisiterRenderPass(std::make_shared<ConeTracingPass>("ConeTracingPass", "./shader/ConeTracingPass.vs", "./shader/ConeTracingPass.fs"));

    //RenderManager::get_instance().regisiterRenderPass(std::make_shared<DefferedConeTracingPass>("DefferedConeTracingPass", "./shader/VXGI/DefferedConeTracingPass.vs", "./shader/VXGI/DefferedConeTracingPass.fs"));




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


