#include "GraphicsManager.h"
//bool mouseCon = false;;
//Camera cam;

//void mouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
//{
//	// Placeholder for mouse button actions
//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//	{
//		mouseCon = true;
//	}
//	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
//	{
//		// Left mouse button released
//		mouseCon = false;
//	}
//}
//void cursorPosCallback(GLFWwindow* pWindow, double xpos, double ypos)
//{
//	if (!mouseCon)return;;
//	static double oldxpos = xpos;
//	static double oldypos = ypos;
//	cam.onCursor(xpos - oldxpos, ypos - oldypos);
//	oldxpos = xpos;
//	oldypos = ypos;
//
//}
//
///*
//   This function serves as the callback parameter for
//	   the glfwSetScrollCallback function, used in the main function
//*/
//void scrollCallback(GLFWwindow* pWindow, double xoffset, double yoffset)
//{
//	cam.onScroll(xoffset, yoffset);
//
//}
//void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
//
//}
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//}

void GraphicsManager::InitializeGraphicsManager() {

	//Create graphics context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Create window to be displayed
	window = glfwCreateWindow(1600.f, 900.f, "GAM300 Graphics test", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window";
		glfwTerminate();
		return;;
	}
	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//Load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Could not load GLAD";
		return;;
	}

	// configure global opengl state
// -----------------------------
		// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, 1600.f, 900.f);

	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	////stbi_set_flip_vertically_on_load(true);

	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetCursorPosCallback(window, cursorPosCallback);
	//glfwSetScrollCallback(window, scrollCallback);
	//glfwSetMouseButtonCallback(window, mouseButtonCallback);

	//Create mesh
	this->cube.CreateMesh();
	this->sphere.CreateMesh();
	this->debugCube.CreateMesh();
	this->debugCircle.CreateMesh();
	
	//Set up frame buffers to use
	//Depth,Gbuffer,FrameBuffer
	frameBuffer.InitializeFBO(1600.f, 900.f);
	depthBuffer.InitializeDepthBuffer();
	gBuffer.InitializeGBuffer();

}

void GraphicsManager::Render() {
	
	//RENDER TO G BUFFER FIRST
	//RENDER SHADOW CAST OBJECTS TO DEPTH BUFFER
	//RENDER TO FRAME BUFFER
	//RENDER TO UI
}