#include"render\RenderPass.h"
#include"render\RenderManager.h"
#include"opengl\Texture.h"
class HBAOPass :public RenderPass
{
public:
	HBAOPass(std::string name, const char* vspath, const char* fspath, const char* gspath = nullptr) : RenderPass(name, vspath, fspath, gspath) {};
	virtual void init();
	virtual void update();
	float* generateNoise();
	std::shared_ptr<Texture> noiseTexture;
	std::shared_ptr<Scene> scene;

};

