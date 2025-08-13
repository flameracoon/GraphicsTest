#pragma once
#include <vector>
#include "Texture.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Material.h"
#define MAX_BONE_INFLUENCE 4

//Forward Declarations
class Model;
class Bone;

//Possibility to optimize this?
struct BoneInfo
{
    glm::mat4 offsetMatrix;      // From ASSIMP during loading
    glm::mat4 finalTransformation{ 1.f }; // Calculated each frame
};

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

class Mesh
{
public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader& shader);
    void PBRDraw(Shader& shader, PBRMaterial const& mat);
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void SetupMesh();
};

class Animation
{
public:
    Animation(const aiAnimation* animation, const aiScene* scene, Model* model);

    float GetTicksPerSecond() const;
    float GetDuration() const;
    const std::string& GetName() const;

    const std::unordered_map<std::string, Bone*>& GetBoneMap() const;
    aiNode* GetRootNode() const;

private:
    float m_Duration;
    float m_TicksPerSecond;
    std::string m_Name;
    std::unordered_map<std::string, Bone*> m_BoneMap;
    aiNode* m_RootNode;
};

class Animator
{
public:
    Animator(Animation* animation);

    void UpdateAnimation(float deltaTime);
    void CalculateBoneTransform(const aiNode* node, const glm::mat4& parentTransform);

    std::vector<glm::mat4> GetFinalBoneMatrices();

private:
    Animation* m_CurrentAnimation;
    float m_CurrentTime;
    std::vector<glm::mat4> m_FinalBoneMatrices;
};

class Bone
{
public:
    Bone(const std::string& name, int id, const aiNodeAnim* channel);

    glm::mat4 Interpolate(float time);

    const std::string& GetName() const;
    int GetID() const;

private:
    std::string m_Name;
    int m_ID;

    std::vector<glm::vec3> m_Positions;
    std::vector<float> m_PositionTimestamps;

    std::vector<glm::quat> m_Rotations;
    std::vector<float> m_RotationTimestamps;

    std::vector<glm::vec3> m_Scales;
    std::vector<float> m_ScaleTimestamps;

    int FindPositionIndex(float animationTime);
    int FindRotationIndex(float animationTime);
    int FindScalingIndex(float animationTime);

    glm::mat4 InterpolatePosition(float time);
    glm::mat4 InterpolateRotation(float time);
    glm::mat4 InterpolateScale(float time);
};

class Model
{
public:
    Model(const char* path)
    {
        LoadModel(path);
    }
    void Draw(Shader& shader);
    void PBRDraw(Shader& shader, PBRMaterial const& pbrMat);
private:
    std::vector<Texture> textures_loaded;
    std::unordered_map<std::string, int> bones_loaded;
    std::vector<BoneInfo> bone_info;
    // model data
    std::vector<Mesh> meshes;
    std::vector<Animation> animations;
    std::string directory;
    

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    void ExtractBoneWeights(aiMesh* mesh, std::vector<Vertex>& vertices);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
        TextureType typeName);
    std::vector<Texture> LoadMaterialTextures(std::string path, TextureType typeName);

    //Utility
    glm::mat4 ConvertToGLM(const aiMatrix4x4& original);
};