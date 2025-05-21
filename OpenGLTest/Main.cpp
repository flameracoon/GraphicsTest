#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include "Shader.h"
#include "stb_image.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>

#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "GraphicsDebuffer.h"
#include "CubeMap.h"
#include "Framebuffer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
std::ostream& operator<< (std::ostream& os, glm::vec2 vec2) {
	os << vec2.x;
	os << ' ' << vec2.y;
	return os;
}
std::ostream& operator <<(std::ostream& os, glm::mat4 mat) {
	std::cout << "Matrix is\n " << mat[0][0] << ' ' << mat[1][0] << ' ' << mat[2][0] << ' ' << mat[3][0] << '\n'
		<< mat[0][1] << ' ' << mat[1][1] << ' ' << mat[2][1] << ' ' << mat[3][1] << '\n'
		<< mat[0][2] << ' ' << mat[1][2] << ' ' << mat[2][2] << ' ' << mat[3][2] << '\n'
		<< mat[0][3] << ' ' << mat[1][3] << ' ' << mat[2][3] << ' ' << mat[3][3] << "\n\n";
	return os;
}
//Define vertices
float vertices[]{
	 0.5f,   0.5f, 0.f,
	 0.5f,  -0.5f, 0.f,
	-0.5f, -0.5f, 0.f,
	-0.5f,  0.5f, 0.f
};

//Define new set of vertices for texture
float texVertices[]{
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f    // top left 
};

unsigned int indices[] = {
	0,1,3,
	1,2,3
};

//Define another set of vertices
float vertices2[]{
	0.f, -0.1f,0.0f, 1.0f, 0.0f, 0.0f,
	0.2f, -0.1f,0.0f, 0.0f, 1.0f, 0.0f,
	0.1f, 0.2f,0.0f,0.0f, 0.0f, 1.0f,
	//second triangle
	0.2f, -0.f,0.0f,1.0f, 0.0f, 0.0f,
	0.3f, 0.f,0.0f, 0.0f, 1.0f, 0.0f,
	0.2f, 0.3f,0.0f,0.0f, 0.0f, 1.0f
};

//Define a cube of vertices
float verticeCube[]{
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,0.0f, 1.0f
};

std::array<glm::vec3,4> cubePositions{
	glm::vec3(0.f,0.f,0.f),
	glm::vec3(4.f,0.f,-6.f),
	glm::vec3(1.6f,3.f,-6.f),
	glm::vec3(-1.f,1.f,-3.f)
};
//Buffer index
unsigned int VBO[4];
unsigned int VAO[4];
unsigned int EBO;


unsigned int vertexShader;
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"layout (location = 1) in vec3 color;\n"

"out vec3 vertexColor;"
"void main()\n"
"{\n"
"   gl_Position = vec4(pos, 1.0);\n"
"   vertexColor = color;\n"
"}\0";

unsigned int fragmentShader;
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 testColor;\n"
"uniform vec4 ourColor;"
"void main()\n"
"{\n"
"   testColor = ourColor;\n"
"}\0";


unsigned int shaderProgram;
unsigned int shaderProgram2;

float persAngle{ 45.f };

//Get camera position
glm::vec3 cameraPos{ 0.f,0.f,5.f };
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

//Set delta time
float lastFrame{ 0.f };
float deltaTime{ 0.f };

//Mouse settings
float yaw = -90, pitch = 0;;
float lastX{ 400 }, lastY{ 300 };
bool firstMouse{ true };

void loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	//std::cout << "Successfully read file" << '\n';
	std::string directory = path.substr(0, path.find_last_of('/'));
}
Camera cam;
bool mouseCon = false;;
void mouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{
	// Placeholder for mouse button actions
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouseCon = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		// Left mouse button released
		mouseCon = false;
	}
}
void cursorPosCallback(GLFWwindow* pWindow, double xpos, double ypos)
{
		if (!mouseCon)return;;
		static double oldxpos = xpos;
		static double oldypos = ypos;
		cam.onCursor(xpos - oldxpos, ypos - oldypos);
		oldxpos = xpos;
		oldypos = ypos;

}

/*
   This function serves as the callback parameter for
	   the glfwSetScrollCallback function, used in the main function
*/
void scrollCallback(GLFWwindow* pWindow, double xoffset, double yoffset)
{
	cam.onScroll(xoffset, yoffset);

}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
}
int main() {

	cam.position.z = -10.f;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create window to be displayed
	GLFWwindow* window = glfwCreateWindow(1600.f, 900.f, "GAM300 Graphics test", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//Load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Could not load GLAD";
		return -1;
	}

	// configure global opengl state
// -----------------------------
		// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, 1600.f, 900.f);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	stbi_set_flip_vertically_on_load(true);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	//loadModel("../Assets/backpack/backpack.obj");
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Create Shader
	Shader skyboxShader("../Assets/Shader/SkyBoxShader/SkyBoxShader.vs", "../Assets/Shader/SkyBoxShader/SkyBoxShader.fs");

	Shader modelShader("../Assets/Shader/LightShader/LightShader.vs", "../Assets/Shader/LightShader/LightShader.fs");
	Shader frameBufferShader("../Assets/Shader/FrameBuffShader/FrameBuffShader.vs", "../Assets/Shader/FrameBuffShader/FrameBuffShader.fs");

	Shader gBufferShader("../Assets/Shader/GBufferShader/GBufferShader.vs", "../Assets/Shader/GBufferShader/GBufferShader.fs");
	Shader deferredLightShader("../Assets/Shader/DeferredLight/DeferredLight.vs", "../Assets/Shader/DeferredLight/DeferredLight.fs");
	Shader normalShader("../Assets/Shader/GBuffNormShader/GBuffNormShader.vs", "../Assets/Shader/GBuffNormShader/GBuffNormShader.fs");
	Shader gBufferPBRShader("../Assets/Shader/GBuffPBRShader/GBuffPBRShader.vs", "../Assets/Shader/GBuffPBRShader/GBuffPBRShader.fs");
	Shader deferredPBRShader("../Assets/Shader/DeferredPBR/DeferredPBR.vs", "../Assets/Shader/DeferredPBR/DeferredPBR.fs");

	Shader debugDepthMapShader("../Assets/Shader/DebugDepthMap/DebugDepthMap.vs", "../Assets/Shader/DebugDepthMap/DebugDepthMap.fs");
	Shader depthMapShader("../Assets/Shader/DepthMap/DepthMap.vs", "../Assets/Shader/DepthMap/DepthMap.fs");
	Shader irradianceShader("../Assets/Shader/IrradianceShader/IrradianceShader.vs", "../Assets/Shader/IrradianceShader/IrradianceShader.fs");
	Model ourModel(std::string{ "../Assets/backpack/backpack.obj" }.c_str());
	//Tmp vertices
		// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	// Set the position of the light


	Texture tex("../Assets/Wife.jpeg", "Tex1");
	//Create texture
	Texture bpDiffTex("../Assets/backpack/diffuse.jpg", "diffuse");
	Texture bpNormTex("../Assets/backpack/normal.png", "normal");
	Texture bpSpecTex("../Assets/backpack/specular.jpg", "metallic");
	Texture bpRoughTex("../Assets/backpack/roughness.jpg", "roughness");
	Texture bpAoTex("../Assets/backpack/ao.jpg", "ao");
	PBRMaterial backpackMat{ &bpDiffTex,&bpSpecTex,&bpRoughTex ,&bpAoTex ,&bpNormTex };
	//Assign backpatMat
	Light light;
	Light light2;
	DirectionalLight dirLight;
	SpotLight spotLight;

	Material material;
	Skybox cubeMap;
	cubeMap.InitializeMap(
		{ "../Assets/SkyBox/night_negx.png", "../Assets/SkyBox/night_posx.png",
		  "../Assets/SkyBox/night_posy.png", "../Assets/SkyBox/night_negy.png",
		  "../Assets/SkyBox/night_posz.png", "../Assets/SkyBox/night_negz.png" });

	float degree = 0.f;
	DebugInit(window);
	glm::vec3 modelPos{0.f};
	glm::vec3 cameraPos{ 0.f };
	//Create a frame buffer
	FrameBuffer frameBuffer;
	frameBuffer.InitializeFBO(1600.f, 900.f);
	frameBuffer.shader = &frameBufferShader;

	//Set up irradiance
	IrradianceMap testIrradiance;
	testIrradiance.InitializeMap();

	FrameBuffer irradianceBuffer;
	irradianceBuffer.InitializeFBO(32.f, 32.f);
	glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, irradianceBuffer.fbo);
	testIrradiance.Render(&irradianceShader, cubeMap);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	frameBuffer.shader = &frameBufferShader;
	GBuffer gBuffer;
	DepthBuffer depthBuffer;
	depthBuffer.InitializeDepthBuffer();
	gBuffer.InitializeGBuffer();
	//Create irradiance map
	
	while (!glfwWindowShouldClose(window))
	{
		// Start ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 🪟 Your ImGui window
		ImGui::Begin("Hello, ImGui!");
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

		ImGui::Text("Use the slider to move the model:");
		ImGui::SliderFloat3("Light Position", &light.position[0], -10.f, 10.f); // vec3 slider
		ImGui::SliderFloat3("Light 2 Position", &spotLight.position[0], -10.f, 10.f); // vec3 slider
		ImGui::SliderFloat3("Light Direction", &dirLight.direction[0], -10.f, 10.f); // vec3 slider

		ImGui::SliderFloat3("Light Color", &spotLight.color[0], -10.0f, 10.0f); // vec3 slider
		ImGui::SliderFloat3("Light Ambient", &spotLight.ambientStrength[0], -10.0f, 10.0f); // vec3 slider
		ImGui::SliderFloat3("Light Diffuse", &spotLight.diffuseStrength[0], -10.0f, 10.0f); // vec3 slider
		ImGui::SliderFloat3("Light Specular", &spotLight.specularStrength[0], -10.0f, 10.0f); // vec3 slider
		ImGui::SliderFloat3("ModelPos Specular", &modelPos[0], -800.0f, 800.0f); // vec3 slider
		ImGui::SliderFloat3("CameraPos", &cameraPos[0], -1600.0f, 1600.0f); // vec3 slider
		ImGui::SliderFloat("Material", &material.reflectivity, 0.f, 1.f); // vec3 slider

		ImGui::End();

		

		////Do deferred shading
		gBuffer.BindGBuffer();

		glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)800, 0.1f, 100.0f);

		view = glm::translate(view, cameraPos / 400.f);
		//* glm::rotate(view, glm::radians(degree), glm::vec3{ 1.f,0.f,0.f });

		degree += 0.1f;

		//Render skybox
		material.SetUniform(&gBufferPBRShader);
		gBufferPBRShader.Use();


		//
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		gBufferPBRShader.SetTrans("projection", cam.CalculatePerspMtx()); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		gBufferPBRShader.SetTrans("view", cam.CalculateViewMtx());
		gBufferPBRShader.SetVec3("cameraPosition", cam.position);
		glm::mat4 model = glm::mat4(1.0f);
	    model = glm::translate(model, modelPos/100.f)*glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		gBufferPBRShader.SetTrans("model", model);
		ourModel.Draw(gBufferPBRShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, { 0.f,0.f,-4.f }) * glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		gBufferPBRShader.SetTrans("model", model);
		ourModel.Draw(gBufferPBRShader);

		gBufferPBRShader.Disuse();
		//glDisable(GL_BLEND);

		//Render to depth map
		glViewport(0, 0, 1600.f, 900.f);
		glCullFace(GL_FRONT);
		dirLight.SetShaderMtrx(&depthMapShader, 0);
		depthMapShader.Use();
		glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer.depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		model = glm::mat4(1.0f);
		model = glm::translate(model, modelPos / 100.f) * glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		depthMapShader.SetTrans("model", model);
		ourModel.PBRDraw(depthMapShader, backpackMat);

		model = glm::mat4(1.0f);
		model = glm::translate(model, { 0.f,0.f,-4.f }) * glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		depthMapShader.SetTrans("model", model);
		ourModel.PBRDraw(depthMapShader, backpackMat);
		depthMapShader.Disuse();
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//gBufferShader.Use();
		//gBufferShader.SetTrans("projection", cam.CalculatePerspMtx()); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		//gBufferShader.SetTrans("view", cam.CalculateViewMtx());
		//gBufferShader.SetVec3("cameraPosition", cam.position);
		//model = glm::mat4{ 1.f };
		//model = glm::translate(model, {1.f,0.f,0.f}) * glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		//gBufferShader.SetTrans("model", model);
		//ourModel.Draw(gBufferShader);
				// reset viewport
		glViewport(0, 0, 1600.f, 900.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render Depth map to quad for visual debugging
		// ---------------------------------------------
		//debugDepthMapShader.Use();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, depthBuffer.RetrieveBuffer());
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBindVertexArray(frameBuffer.vaoId);
		//glDrawElements(GL_TRIANGLE_STRIP, frameBuffer.drawCount, GL_UNSIGNED_SHORT, NULL);
		//glDisable(GL_BLEND);
	
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.f, 0.0f, 0.f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		light.color = glm::vec3(1.f, 1.f, 1.f);
		light.SetUniform(&deferredPBRShader, 0);
		light2.SetUniform(&deferredPBRShader, 1);

		spotLight.SetUniform(&deferredPBRShader, 0);
		dirLight.SetUniform(&deferredPBRShader, 0);
		dirLight.SetShaderMtrx(&deferredPBRShader, 0);
		testIrradiance.RenderCube(&skyboxShader, glm::mat4(glm::mat3(cam.GetVieMtx())), cam.GetPerspMtx());

		deferredPBRShader.Use();
		deferredPBRShader.SetVec3("lightAmbience", Light::ambientStrength);

		deferredPBRShader.SetTrans("view", cam.CalculateViewMtx());
		deferredPBRShader.SetInt("pointLightNo", 1);
		deferredPBRShader.SetInt("dirLightNo", 0);
		deferredPBRShader.SetInt("spotLightNo", 1);

		gBuffer.UseGTextures();
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, testIrradiance.RetrieveID());
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, depthBuffer.RetrieveBuffer());
		glUniform1i(glGetUniformLocation(deferredPBRShader.ID, "gPosition"), 0);  // Bind to GL_TEXTURE0
		glUniform1i(glGetUniformLocation(deferredPBRShader.ID, "gNormal"), 1);    // Bind to GL_TEXTURE1
		glUniform1i(glGetUniformLocation(deferredPBRShader.ID, "gAlbedoSpec"), 2); // Bind to GL_TEXTURE2
		glUniform1i(glGetUniformLocation(deferredPBRShader.ID, "gReflect"), 3);
		glUniform1i(glGetUniformLocation(deferredPBRShader.ID, "gMaterial"), 4);

		//material.SetUniform(&deferredLightShader);
		

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindVertexArray(frameBuffer.vaoId);
		glDrawElements(GL_TRIANGLE_STRIP, frameBuffer.drawCount, GL_UNSIGNED_SHORT, NULL);
		glDisable(GL_BLEND);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer.RetrieveBuffer());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, 1600, 900, 0, 0, 1600, 900, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Render Light
		//glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.fbo); // back to default
		//glClearColor(0.f, 0.0f, 0.f, 0.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_DEPTH_TEST);
		//light.SetUniform(&modelShader, 0);
		//material.SetUniform(&modelShader);

		//tex.Use();
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.RetrieveID());

		//modelShader.Use();
		//modelShader.SetTrans("projection", cam.CalculatePerspMtx()); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		//modelShader.SetTrans("view", cam.CalculateViewMtx());
		//modelShader.SetVec3("cameraPosition", cam.position);

		////// render the loaded model
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, modelPos/100.f)*glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		//modelShader.SetTrans("model", model);
		//ourModel.Draw(modelShader);

		//model = glm::mat4{ 1.f };
		//model = glm::translate(model, {1.f,0.f,0.f}) * glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		//modelShader.SetTrans("model", model);
		//ourModel.Draw(modelShader);
		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//glBindVertexArray(0);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		//glClearColor(0.f, 0.0f, 1.f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
		//frameBuffer.Render();

		//Draw vertex
		glfwSwapBuffers(window);
		glfwPollEvents();


	}
	//Delete programs
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram2);

	glfwTerminate();
	return 0;
}