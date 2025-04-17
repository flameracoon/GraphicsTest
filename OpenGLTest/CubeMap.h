#pragma once
#include "Texture.h"
#include <vector>
#include <string>
#include "Shader.h"

class CubeMap {
public:
	void InitializeMap(std::vector<std::string>);
	void LoadCubeTexture(std::vector<std::string>);
	void LoadCubeModel();
	int RetrieveID();
protected:
	GLuint VAO, VBO;
	GLuint texID;
};
class Skybox :public CubeMap {

public:
	void Render(Shader*, glm::mat4 const& view, glm::mat4 const& projection);
};