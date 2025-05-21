#include "Model.h"
Mesh::Mesh(std::vector<Vertex> newVert, std::vector<unsigned int> newIndices, std::vector<Texture> newTextures)
	:vertices{newVert}
	,indices{newIndices}
	,textures{newTextures}
{
    //set up mesh based on data
    SetupMesh();
}

void Mesh::SetupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
    glBindVertexArray(0);
}


void Mesh::Draw(Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    unsigned int roughNr = 1;

    //Set material textures
    //std::cout << textures.size() << '\n';
    std::string number;
    std::string name{};
    for (unsigned int i = 0; i < textures.size(); i++)
    {
       glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
       // retrieve texture number (the N in diffuse_textureN)
       TextureType texType = textures[i].RetrieveType();
       switch (texType) {
        case DIFFUSE:
            number = "texture_diffuse"+ std::to_string(diffuseNr++);
            break;;
        case SPECULAR:
            number = "texture_specular"+std::to_string(specularNr++);
            break;;
        case NORMAL:
            number = "texture_normal"+std::to_string(normalNr++);
            break;;
        case HEIGHT:
            number = "texture_ao" + std::to_string(heightNr++);
            break;;
        case ROUGHNESS:
            number = "texture_roughness" + std::to_string(roughNr++);
            break;;
       }
        shader.SetInt((number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].RetrieveTexture());
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void Mesh::PBRDraw(Shader& shader, PBRMaterial const& mat) {


    //Set material textures
    //std::cout << textures.size() << '\n';
    std::string number;
    std::string name{};
    //Bind albedo
    glActiveTexture(GL_TEXTURE0); // activate proper texture unit before binding
    shader.SetInt("texture_diffuse1", 0);
    glBindTexture(GL_TEXTURE_2D, mat.albedo->RetrieveTexture());
    //Bind sepcular
    glActiveTexture(GL_TEXTURE1); // activate proper texture unit before binding
    shader.SetInt("texture_specular1", 1);
    glBindTexture(GL_TEXTURE_2D, mat.specular->RetrieveTexture());
    //Bind normal
    glActiveTexture(GL_TEXTURE2); // activate proper texture unit before binding
    shader.SetInt("texture_normal1", 2);
    glBindTexture(GL_TEXTURE_2D, mat.normal->RetrieveTexture());
    //Bind Metallic map
    glActiveTexture(GL_TEXTURE4); // activate proper texture unit before binding
    shader.SetInt("texture_ao1", 4);
    glBindTexture(GL_TEXTURE_2D, mat.ao->RetrieveTexture());
    //Bind roughness
    glActiveTexture(GL_TEXTURE5); // activate proper texture unit before binding
    shader.SetInt("texture_roughness1", 5);
    glBindTexture(GL_TEXTURE_2D, mat.roughness->RetrieveTexture());
    
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void Model::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}
void Model::PBRDraw(Shader& shader, PBRMaterial const& pbrMat) {
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].PBRDraw(shader,pbrMat);
}


std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].RetrieveName().c_str(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        { 
            std::cout << "Attempting load on:" << std::string{ directory + '/' + str.C_Str() }.c_str() << '\n';
            textures.push_back({ std::string{directory + '/' + str.C_Str()}.c_str(),str.C_Str() });
            textures.back().SetType(typeName);
            textures_loaded.push_back(textures.back());
         }
    }
    return textures;
}
std::vector<Texture> Model::LoadMaterialTextures(std::string fileName, TextureType typeName) {

    std::vector<Texture> textures;
    for (unsigned int j = 0; j < textures_loaded.size(); j++)
    {
        if (std::strcmp(textures_loaded[j].RetrieveName().c_str(), fileName.c_str()) == 0)
        {
            textures.push_back(textures_loaded[j]);
            return textures;;
        }
    }
    std::cout << "Attempting load on:" << std::string{ directory + '/' + fileName }.c_str() << '\n';
    textures.push_back({ std::string{directory + '/' + fileName}.c_str(),fileName});
    textures.back().SetType(typeName);
    textures_loaded.push_back(textures.back());

    return textures;
}
//Process meshes
Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) //Check if got texturre coords
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    } 
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    std:: vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, SPECULAR);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, NORMAL);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 3. AO maps
    std::vector<Texture> heightMaps = LoadMaterialTextures("ao.jpg",HEIGHT);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    std::vector<Texture> roughnessMaps = LoadMaterialTextures("roughness.jpg", ROUGHNESS);
    textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

//Process all nodes of the mesh
void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}


void Model::LoadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);
    std::cout << "Loaded model";

}