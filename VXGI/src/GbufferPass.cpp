#include"GbufferPass.h"


void GbufferPass::init() {

    //get scene£»
    scene = RenderManager::get_instance().getScene();


    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    color =  std::make_shared<Texture>(GL_RGBA32F,GL_FLOAT, GL_RGBA,scene->SCR_WIDTH,scene->SCR_HEIGHT);
    normal = std::make_shared<Texture>(GL_RGB16F, GL_FLOAT, GL_RGB, scene->SCR_WIDTH, scene->SCR_HEIGHT);
    position = std::make_shared<Texture>(GL_RGBA32F, GL_FLOAT, GL_RGB, scene->SCR_WIDTH, scene->SCR_HEIGHT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color->ID, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal->ID, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, position->ID, 0);
    
    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);


    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scene->SCR_WIDTH, scene->SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);


    RenderManager::get_instance().regisiterTexture("gbufferColor", color->ID);
    RenderManager::get_instance().regisiterTexture("gbufferNormal", normal->ID);
    RenderManager::get_instance().regisiterTexture("gbufferPosition", position->ID);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shader->use();
    shader->setInt("rsmPosition", 2);
    

}

void GbufferPass::update() {
    shader->use();
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glViewport(0, 0, scene->SCR_WIDTH, scene->SCR_HEIGHT);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glm::mat4 model = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f)), glm::vec3(0.0f, 0.0f, 0.0f));
    shader->setMat4("model", model);
    shader->setMat4("u_LightVPMatrix", scene->light.getLightVPMatrix());
    shader->setVec3("u_LightDir", scene->light.LightDir);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, RenderManager::get_instance().getTexture("rsmPosition"));
   


    scene->Draw(*shader);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}