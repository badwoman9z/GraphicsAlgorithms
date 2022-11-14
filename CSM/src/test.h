#pragma once
#include"render/RenderManager.h"
#include"render/RenderPass.h"
#include"CSMPass.h"
#include "CSMScreenQuadPass.h"
namespace CSM {
	void registerPipeLine() {
		RenderManager::get_instance().regisiterRenderPass(std::make_shared<CSMPass>("CSMPass", "./shader/CascadedShadowMap.vs", "./shader/CascadedShadowMap.fs", "./shader/CascadedShadowMap.gs"));
		RenderManager::get_instance().regisiterRenderPass(std::make_shared<CSMScreenQuadPass>("CSMScreenQuadPass", "./shader/CSMScreenQuadPass.vs", "./shader/CSMScreenQuadPass.fs"));
	}
}