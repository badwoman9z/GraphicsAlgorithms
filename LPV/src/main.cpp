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
#include"render/RenderManager.h"
#include"GbufferPass.h"
#include"ScreenQuadPass.h"
#include"RSMBufferPass.h"
#include"LightInjectPass.h"
#include"AABBDebugPass.h"
#include"GeometryInjectPass.h"
#include"PropagationPass.h"
#include"IndirectLightPass.h"
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
    for (int i = 0; i < 8; i++) {
        glm::ivec3 t = glm::ivec3(i, i >> 1, i >> 2) & glm::ivec3(1);
        std::cout << t.x << t.y << t.z << std::endl;

    }


    GUI::Init();

    if (GUI::Create(SCR_WIDTH,SCR_HEIGHT) != 1) {
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
    
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(Camera(glm::vec3(0, 0, 0)), Light(glm::vec3(-0.15, -1.13, -0.58), glm::vec3(1.0)));
    //scene->LoadScene("d:/user/wkx/desktop/grapincs/rendertoy/gi/model/sponza.obj");

    scene->LoadModel("d:/user/wkx/desktop/grapincs/rendertoy/gi/model/sponza1.obj");
    scene->getOrCreateBounding();
    RenderManager::get_instance().regisiterScene(scene);

    RenderManager::get_instance().regisiterRenderPass(std::make_shared<RSMBufferPass>("RSM", "./shader/RSMBufferPass.vs", "./shader/RSMBufferPass.fs"));

    RenderManager::get_instance().regisiterRenderPass(std::make_shared<GbufferPass>("Gbuffer", "./shader/gbuffer.vs", "./shader/gbuffer.fs"));
    
    RenderManager::get_instance().regisiterRenderPass(std::make_shared<LightInjectPass>("LightInject", "./shader/LightInjectPass.vs"  , "./shader/LightInjectPass.fs", "./shader/LightInjectPass.gs"));

   //RenderManager::get_instance().regisiterRenderPass(std::make_shared<GeometryInjectPass>("GeometryInjectPass", "./shader/GeometryInjectPass.vs", "./shader/GeometryInjectPass.fs"));


    RenderManager::get_instance().regisiterRenderPass(std::make_shared<PropagationPass>("PropagationPass", "./shader/PropagationPass.vs", "./shader/PropagationPass.fs", "./shader/PropagationPass.gs"));
    
    RenderManager::get_instance().regisiterRenderPass(std::make_shared<IndirectLightPass>("IndirectLightPass", "./shader/IndirectLightPass.vs", "./shader/IndirectLightPass.fs"));
    
    RenderManager::get_instance().regisiterRenderPass(std::make_shared<ScreenQuadPass>("ScreenQuad", "./shader/ScreenQuadPass.vs", "./shader/ScreenQuadPass.fs"));
   
    
    //RenderManager::get_instance().regisiterRenderPass(std::make_shared<AABBDebugPass>("AABBDebug", "./shader/AABBDebug.vs", "./shader/AABBDebug.fs", "./shader/AABBDebug.gs"));

    


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


