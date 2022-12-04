#include"render\RenderPass.h"
#include"render\RenderManager.h"
#include"opengl\Texture.h"
#include <random>
class SSAOPass:public RenderPass
{
public:
	SSAOPass(std::string name, const char* vspath, const char* fspath, const char* gspath = nullptr) : RenderPass(name, vspath, fspath, gspath) {
	
		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
		std::default_random_engine generator;
		for (GLuint i = 0; i < 64; ++i)
		{
			glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			GLfloat scale = GLfloat(i) / 64.0;

			// Scale samples s.t. they're more aligned to center of kernel
			scale = lerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}
	
	};
	virtual void init();
	virtual void update();
	std::vector<glm::vec3> ssaoKernel;
	std::shared_ptr<Texture> noiseTexture;
	std::shared_ptr<Scene> scene;
	float lerp(GLfloat a, GLfloat b, GLfloat f)
	{
		return a + f * (b - a);
	}



};

