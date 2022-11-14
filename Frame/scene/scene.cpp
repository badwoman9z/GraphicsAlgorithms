#include"scene.h"
#include"opengl/Texture.h"
#include"mesh.h"
#include<map>
std::string getBasePath(const std::string& path)
{
    size_t pos = path.find_last_of("\\/");
    return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
}
//void Scene::LoadScene(const std::string& pFile)
//{
//    Assimp::Importer importer;
//
//    const aiScene* scene = importer.ReadFile(pFile,
//        aiProcess_CalcTangentSpace |
//        aiProcess_Triangulate |
//        aiProcess_JoinIdenticalVertices);
//
//    if (nullptr == scene) {
//        std::cout << "Scene load failed!: " << pFile << std::endl << importer.GetErrorString() << std::endl;
//        return;
//    }
//    std::cout << "Loading Scene: " << pFile << std::endl;
//
//
//    std::string folder = getBasePath(pFile);
//
//    std::map<std::string, std::shared_ptr<Texture>> textureMap;
//    std::vector<std::shared_ptr<Material>> materials;
//
//    std::cout << "mNumMaterials=" << scene->mNumMaterials << std::endl;
//    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
//    {
//        aiMaterial* aimaterial = scene->mMaterials[i];
//
//        auto mat = std::make_shared<Material>();
//
//        aiString path;
//        aiColor4D diffuse;
//
//        if (AI_SUCCESS == aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path))
//        {
//            
//            mat->albedo = std::make_shared<Texture>((folder + path.data).c_str());
//        }
//        //if (AI_SUCCESS == aimaterial->GetTexture(aiTextureType_SPECULAR, 0, &path))
//        //{
//        //    std::cout << (folder + path.data).c_str() << std::endl;
//        //    mat->specular = std::make_shared<Texture>((folder + path.data).c_str());
//        //}
//        aimaterial->GetTexture(aiTextureType_SPECULAR, 0, &path);
//        std::cout << (folder + path.data).c_str() << std::endl;
//
//        float metallic = 1.0f;
//        mat->metallic = metallic;
//
//        float roughness = 0.0f;
//        mat->roughness = roughness;
//        materials.push_back(mat);
//    }
//
//    std::cout << "mNumMeshes=" << scene->mNumMeshes << std::endl;
//    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
//    {
//        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(scene->mMeshes[i]);
//        mesh->material = materials[scene->mMeshes[i]->mMaterialIndex];
//        AddObject(mesh);
//    }
//
//    std::cout << "Scene Load Finished." << std::endl;
//}
void Scene::LoadModel(const std::string& pfile)
{
    models.push_back(std::make_shared<Model>(pfile));
}
void Scene::LoadModel(const std::string& pfile, int num)
{
    models.push_back(std::make_shared<Model>(pfile,num));
}
void Scene::Draw(Shader shader)
{
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    for (int i = 0; i < models.size(); i++) {

        models[i]->Draw(shader);
    }

}
void Scene::DrawInstance(Shader shader)
{
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    for (int i = 0; i < models.size(); i++) {

        models[i]->DrawInstance(shader);
    }
}
void Scene::Update() {

    view = camera->GetViewMatrix();

}
void Scene::Init() {
    //if (objects.size() != 0) {
    //    bounding = getOrCreateBounding();
    //}

}