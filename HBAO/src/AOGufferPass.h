#include"render\RenderPass.h"
#include"opengl\Texture.h"
#include"scene\scene.h"
#include"render\RenderManager.h"
class AOGufferPass:public RenderPass
{
public:
	AOGufferPass(std::string name, const char* vspath, const char* fspath, const char* gspath = nullptr) : RenderPass(name, vspath, fspath, gspath) {};
	
	virtual void init();
	virtual void update();
	unsigned int FBO;
	unsigned int RBO;
	std::shared_ptr<Texture> gbufferTexture;
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Texture> depthTexture;
	std::shared_ptr<Texture> normal;
};

