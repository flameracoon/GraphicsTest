/*!
@file         glapp.cpp
@author  	  pghali@digipen.edu
@co-author	  parminder.singh@digipen.edu , gabe.ng@digipen.edu
@date    	  04/06/2024

This file implements functionality useful and necessary to build OpenGL
applications including use of external APIs such as GLFW to create a
window and start up an OpenGL context and to extract function pointers
to OpenGL implementations.

*//*__________________________________________________________________________*/
//Have implemented bonus task
/*                                                                   includes
----------------------------------------------------------------------------- */
#include <glapp.h>
#include <iostream>
#include <cmath>
#include <sstream>
#include <ctime>
#include <chrono>
#include <random>
#include <glm/gtc/type_ptr.hpp>
// for glm::value_ptr
/*                                                   objects with file scope
----------------------------------------------------------------------------- */
double previousTime{0};
//float rVal{ 0.f }, gVal{ 1.f }, bVal{0.f};
//float rVal2{ 1.f }, gVal2{ 0.f }, bVal2{ 0.f };

bool changeCon{ false };
double backGroundDuration{ 0. };

std::vector<GLApp::GLViewport> GLApp::viewportList(1);

std::map<std::string, GLApp::GLObject>GLApp::objects;
std::map<std::string, GLApp::GLModel>GLApp::models;
std::map<std::string, GLSLShader>GLApp::shaderPrograms;
//define random
std::random_device rd;
std::default_random_engine dre3(rd());
std::uniform_real_distribution<GLfloat> randDist(-1.f, 1.f);
std::uniform_real_distribution<GLfloat> randSize(50.f, 400.f);

GLfloat const worldHalfExtent{ 5000.0f };
//Set game world size
int counter{ 0 };
GLuint shaderCounter{ 0 }, boxCounter{ 0 }, mysteryCounter{0};
GLboolean previousState{ GL_FALSE };

GLApp::Camera2D GLApp::camera2D;


/*  _________________________________________________________________________*/
/*! init

@parm std::string
The mesh file to be read from

Reads from the mesh file and saves data in it into the GLModel

*/
void GLApp::GLModel::init(std::string meshFile) {
	//draw file
	std::ifstream ifs{ meshFile ,std::ios::in };
	if (!ifs) {
		std::cout << "Error, cannot open file: " << meshFile << '\n';
		exit(EXIT_FAILURE);
	}

	std::string modelName;
	std::vector<glm::vec2> positionList;


	std::vector<GLushort>indices;
	//Loop till end of file
	while (!ifs.eof()) {
		std::string line;
		getline(ifs, line);
		//Convert to string stream
		std::istringstream lineStream{ line };
		char currentChar;
		lineStream >> currentChar;

		switch (currentChar) {
			case 'n':
				lineStream >> modelName;
				break;;
			case 'v':
				float x, y;
				lineStream >> x;
				lineStream >> y;
				positionList.emplace_back(glm::vec2{ x,y });
				break;;
			default:
				GLushort currentIndice;
				while (lineStream >> currentIndice) {
					indices.emplace_back(currentIndice);
				}
				break;;
		}
		if (currentChar == 't') {
			primitive_type = GL_TRIANGLES;
			
		}
		else if (currentChar == 'f') {
			primitive_type = GL_TRIANGLE_FAN;
		}
	}
	//Set up VAO and VBO
	GLuint VBO;
	glCreateBuffers(1, &VBO);
	GLsizei vertexAttributeSize = sizeof(glm::vec2);
	GLsizei  vertexDataSize = vertexAttributeSize * static_cast<GLsizei>(positionList.size());
	glNamedBufferStorage(VBO, vertexDataSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(VBO, 0, vertexDataSize, positionList.data());
	//Set up VAO
	glCreateVertexArrays(1, &vaoid);
	glEnableVertexArrayAttrib(vaoid, 0);
	glVertexArrayVertexBuffer(vaoid, 1, VBO, 0, vertexAttributeSize);
	glVertexArrayAttribFormat(vaoid, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 0, 1);
	//Set draw count
	drawCount = static_cast<GLuint>(indices.size());
	GLuint eboId;
	glCreateBuffers(1, &eboId);
	//Save data
	glNamedBufferStorage(eboId, sizeof(GLushort) * drawCount, reinterpret_cast<GLvoid*>(indices.data()), GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vaoid, eboId);

	primitiveCount = 2;
	glBindVertexArray(0);
}





 /*  _________________________________________________________________________*/
 /*! init

 @param none

 @return none

 Initializes GLObject and its variables
	 World coordinates would be tabulates with
	 Position coordiantes with the game size of 10000X10000
	 Scale with size of 50-400
	 Rotation which would be randomly generates
	 These values would then be condenced into mdlToNDC matrix whic is then passed to the vertex shader
	 Shader refrence would choose whichever shader is currently active
	 Model reference would be a random between box and mystery
 */
 void GLApp::GLObject::init() {
	
	 //Initialize values
	 //Assign shader
	 glm::mat3 offSet{ 1.f }, scaleMat{ 1.f }, rotMat{ 1.f }, transMat{ 1.f };


	 scaleMat[0][0] = scaling.x;
	 scaleMat[1][1] = scaling.y;
	
	 rotMat[0][0] = cos(orientation.x);
	 rotMat[1][0] = -sin(orientation.x);
	 rotMat[0][1] = sin(orientation.x);
	 rotMat[1][1] = cos(orientation.x);

	 transMat[2][0] = position.x;
	 transMat[2][1] = position.y;


	 mdlToNDC = camera2D.worldToNDC * transMat * rotMat * scaleMat;




}
/*  _________________________________________________________________________*/
/*! update

@parm GLdouble
takes in the deltatime to be used with rotation

@return none

This function updates the object's rotation along the Z-axis based on delta time and render speed
From there a new mdl is tabulated

*/
 void GLApp::GLObject::update(GLdouble deltaTime) {
	 glm::mat3 offSet{ 1.f }, scaleMat{ 1.f }, rotMat{ 1.f }, transMat{ 1.f };


	 scaleMat[0][0] = scaling.x;
	 scaleMat[1][1] = scaling.y;

	 orientation.x += orientation.y*static_cast<GLfloat>(deltaTime);
	 rotMat[0][0] = cos(orientation.x);
	 rotMat[1][0] = -sin(orientation.x);
	 rotMat[0][1] = sin(orientation.x);
	 rotMat[1][1] = cos(orientation.x);

	 transMat[2][0] = position.x;
	 transMat[2][1] = position.y;


	 mdlToNDC =camera2D.worldToNDC * transMat * rotMat * scaleMat;

 }
 /*  _________________________________________________________________________*/
 /*! draw

 @param none

 @return none

 Sets the unifrom of the shader program in use by the object with the mdl matrix
 It would draw the model depending on its corresponding model type
 */
 void GLApp::GLObject::draw() const{

	 shaderRef->second.Use();
	 GLint uniformLocation = glGetUniformLocation(
		 shaderRef->second.GetHandle(), "uModel_to_NDC");
	 if (uniformLocation < 0) {
		 std::cout << "Uniform variable doesn't exist!!!\n";
		 std::exit(EXIT_FAILURE);
	 }
	 GLint colorLocation = glGetUniformLocation(
		 shaderRef->second.GetHandle(), "uColor");
	 if (colorLocation < 0) {
		 std::cout << "Uniform variable doesn't exist!!!\n";
		 std::exit(EXIT_FAILURE);
	 }
	
	 //Render program
	 glUniformMatrix3fv(uniformLocation, 1, GL_FALSE,
		 glm::value_ptr(GLApp::GLObject::mdlToNDC));
	 glUniform3fv (colorLocation, 1,
		 &color[0]);

	 glBindVertexArray(modelRef->second.vaoid);
	 glDrawElements(modelRef->second.primitive_type, modelRef->second.drawCount, GL_UNSIGNED_SHORT, NULL);
	 /*std::cout << GLApp::GLObject::mdlToNDC[0][0];
	 std::cout << GLApp::GLObject::mdlToNDC[0][1];
	 std::cout << GLApp::GLObject::mdlToNDC[0][2];
	 std::cout << GLApp::GLObject::mdlToNDC[1][1]<<'\n';*/

	// std::cout << shaderRef->first<<'\n';
	 glBindVertexArray(0);
	 shaderRef->second.UnUse();
 }
 /*  _________________________________________________________________________*/
/*! InsertShaderPgm

@parm std::string
The name of the shader program to be inserted into the static map

@parm std::string
The path to the vertex shader

@parm std::string
The path to the fragment shadder
@return none

Compiles and links shaders to the shader program
Generates a new shader program if name dosent exist in the map,
else function ends

*/
 void GLApp::InsertShaderPgm(std::string shaderPgmName, std::string vertexShader, std::string fragmentShader) {

	 //Check if shader name already exists within scene
	 std::map<std::string, GLSLShader>::iterator it =GLApp::shaderPrograms.find(shaderPgmName);
	 if (it != GLApp::shaderPrograms.end()) return;

	 std::vector<std::pair<GLenum, std::string>> shaderFiles{
		std::make_pair(GL_VERTEX_SHADER, vertexShader),
		std::make_pair(GL_FRAGMENT_SHADER, fragmentShader)
	 };
	 GLSLShader shaderProgram;
	// AUTOMATION_HOOK_SHADER(shaderProgram, shaderFiles);

	 shaderProgram.CompileLinkValidate(shaderFiles);
	 if (GL_FALSE == shaderProgram.IsLinked()) {
		 std::cout << "Unable to compile/link/validate shader programs\n";
		 std::cout << shaderProgram.GetLog() << "\n";
		 std::exit(EXIT_FAILURE);
	 }
	 GLApp::shaderPrograms[shaderPgmName] = shaderProgram;
 }

 /*  _________________________________________________________________________*/
/*! InitScene

@parm std::string
The scene file to be read from

Reads the scene file and generates objects/ models based on it

*/
 void GLApp::InitScene(std::string sceneFile) {
	//Stream file
	 std::ifstream ifs{ sceneFile ,std::ios::in };
	 if (!ifs) {
		 std::cout << "Error, cannot open file: " << sceneFile << '\n';
		 exit(EXIT_FAILURE);
	 }
	 //Go to the begiing of the file
	 ifs.seekg(0, std::ios::beg);

	 std::string line;
	 getline(ifs, line);
	 //Convert to string stream
	 std::istringstream lineStream{ line };
	 int objectCount;
	 lineStream >> objectCount;
	 while (objectCount--) {
		 getline(ifs, line);
		 std::istringstream lineModelName{ line };
		 std::string modelName;
		 lineModelName >> modelName;

		 //Check if model exists within the map
		 std::map<std::string, GLModel>::iterator it = GLApp::models.find(modelName) ;
		 //If model dosent exist, add a new one to pre existing models
		 if (it == GLApp::models.end()) {
			 GLModel newModel;
			 std::string fileName{"../meshes/"+modelName + ".msh"};
			 newModel.init(fileName);
			 models[modelName] = newModel;
		 }
		 //Add object
		 getline(ifs, line);
		 std::istringstream objectLineName{ line };
		 std::string objectName;
		 objectLineName >> objectName;

		 //Find shader
		 getline(ifs, line);
		 std::istringstream shaderLine{ line };
		 std::string shaderSource, shaderVertex, shaderFragment;
		 shaderLine >> shaderSource;
		 shaderLine >> shaderVertex;
		 shaderLine >> shaderFragment;
		 GLApp::InsertShaderPgm(shaderSource, shaderVertex, shaderFragment);


		 GLfloat val1, val2, val3;
		 GLObject newObject;

		 //Insert shader reference
		 newObject.shaderRef = shaderPrograms.find(shaderSource);
		 newObject.modelRef = models.find(modelName);

		 //Extract color
		 getline(ifs, line);
		 std::istringstream objectColor{ line };
		 objectColor >> val1;
		 objectColor >> val2;
		 objectColor >> val3;
		 newObject.color = glm::vec3{ val1,val2,val3 };

		 //Extract scale
		 getline(ifs, line);
		 std::istringstream scaleLine{ line };
		 scaleLine >> val1;
		 scaleLine >> val2;
		 newObject.scaling = glm::vec2{ val1,val2 };

		 //Extract rotation
		 getline(ifs, line);
		 std::istringstream rotationLine{ line };
		 rotationLine >> val1;
		 rotationLine >> val2;
		 newObject.orientation = glm::vec2{glm::radians(val1),glm::radians(val2) };

		 //Extract position
		 getline(ifs, line);
		 std::istringstream positionLine{ line };
		 positionLine >> val1;
		 positionLine >> val2;
		 newObject.position = glm::vec2{ val1,val2 };


		 objects[objectName] = newObject;
		
	 }
	 
 }

 /*  _________________________________________________________________________*/
/*! init

@parm GLFWwindow*
The window refernced by the camera

@parm GLObject*
The camera object

Initializes the camera object with the relevant oritentation, position and matrix

*/
 void GLApp::Camera2D::init(GLFWwindow* window, GLObject* ptr) {
	 pgo = ptr;
	 up = glm::vec2{ -sin(pgo->orientation.x),cos(pgo->orientation.x) };
	 right= glm::vec2{ cos(pgo->orientation.x),sin(pgo->orientation.x) };


		 viewMat[2][0] = -pgo->position.x;
		 viewMat[2][1] = -pgo->position.y;
	 

	

	 GLsizei fb_width, fb_height;
	 glfwGetFramebufferSize(window, &fb_width, &fb_height);
	 ar = static_cast<GLfloat>(fb_width) / height;

	 camwinToNDC[0][0] = 2 / (ar* height);
	 camwinToNDC[1][1]= 2 / (fb_width /ar);
	 worldToNDC = camwinToNDC * viewMat;
 }

 /*  _________________________________________________________________________*/
/*! update

@parm GLFWwindow*
The window refernced by the camera

@return none

Updates the camera depending on the key states
 H and K increases/decreases the oritentation respectively
 U moves it in the direction its facing
 V chnanges the cam mode
 Z zooms out, would zoom in once max is reached and vice versa
  @return none
*/
 void GLApp::Camera2D::update(GLFWwindow* window) {

	
	 up = glm::vec2{ -sin(pgo->orientation.x),cos(pgo->orientation.x) };
	 right = glm::vec2{ cos(pgo->orientation.x),sin(pgo->orientation.x) };

	 if (GLHelper::keyStateH) {
		 pgo->orientation.x += pgo->orientation.y;
		 pgo->orientation.x = glm::degrees(pgo->orientation.x) >360 ? glm::radians(glm::degrees(pgo->orientation.x) - 360) : pgo->orientation.x;

	 }
	 if (GLHelper::keyStateK) {
		 pgo->orientation.x -= pgo->orientation.y;
		 pgo->orientation.x = glm::degrees(pgo->orientation.x) < -360 ? glm::radians(glm::degrees(pgo->orientation.x)+360) : pgo->orientation.x;
	 }
	
	 if (GLHelper::keyStateU) {
		 pgo->position.x += linear_speed * up.x;
		 pgo->position.y += linear_speed * up.y;
	 }
	 if (GLHelper::keyStateZ) {
		 if (max_height<height || min_height>height) {
			 heightChangeDir *= -1;
		 }
		 height += heightChangeDir * heightChangeVal;
	 }
	 if (!GLHelper::keyStateV) {

		 viewMat[0][0] = 1;
		 viewMat[0][1] = 0;
		 viewMat[1][0] = 0;
		 viewMat[1][1] = 1;

		 viewMat[2][0] = -pgo->position.x;
		 viewMat[2][1] = -pgo->position.y;
	 }
	 else {
		 viewMat[0][0] = right.x;
		 viewMat[0][1] = -right.y;
		 viewMat[1][0] = right.y;
		 viewMat[1][1] = right.x;

		 viewMat[2][0] = -1 * (right.x * pgo->position.x + right.y * pgo->position.y);
		 viewMat[2][1] = -1 * (-right.y * pgo->position.x + right.x * pgo->position.y);
	 }

	 GLsizei fb_width, fb_height;
	 glfwGetFramebufferSize(window, &fb_width, &fb_height);
	 ar = static_cast<GLfloat>(fb_width) / static_cast<GLfloat>(fb_height);

	 camwinToNDC[0][0] = 2 / (ar * height);

	 camwinToNDC[1][1] = 2 / static_cast<GLfloat>(height);
		 std::cout << fb_width << ' ' << fb_height << ' ' << 2 / height<<'\n';

	 worldToNDC = camwinToNDC * viewMat;
	 pgo->init();
 }
 /*  _________________________________________________________________________*/
 /*! init

 @param none

 @return none

 Initializes values needed to render the background and set up the model used to render the
 triangle as well as setting the randomized seed
 Initalizes shaders and models needed in the program

 */

void GLApp::init() {


	//Set random seed
	std::string buffer = {};

	//Get initial time
	auto t1 = std::chrono::high_resolution_clock::now();
	buffer.append(std::to_string(glfwGetTime()));
	std::random_device rd;
	buffer.append(std::to_string(rd()));
	auto t2 = std::chrono::high_resolution_clock::now();

	// Getting number of milliseconds as a double. 
	std::chrono::duration<double, std::milli> ms_double = t2 - t1;

	//Get current functione execution time
	buffer.append(std::to_string(ms_double.count()));
	std::hash<std::string> str_hash;
	srand(static_cast<unsigned int>(str_hash(buffer)));

    // Set clear color
	glClearColor(1.f, 1.f,1.f, 1.f);

	//Set previosu time as current time
	previousTime = glfwGetTime();

	//Add new viewport
	GLApp::viewportList.push_back({ 0, 0, GLHelper::width, GLHelper::height });



	//Set point size
	glPointSize(10.f);

	InitScene("../scenes/tutorial-4.scn");
	//Initialize camera
	GLApp::camera2D.init(GLHelper::ptr_window, &GLApp::objects.at("Camera"));
	for (auto& obj : GLApp::objects) {
		obj.second.init(); // call member function GLObject::init
	}
}
/*  _________________________________________________________________________*/
/*! update

@param none

@return none

 Updates the viewport for it to be resized
 Updates the camera based on inputs
 Loops through each object and updates everyone except camera
*/
void GLApp::update() {
	GLint w{ GLHelper::width }, h{ GLHelper::height };
	static GLint old_w{}, old_h{};
	// update viewport settings in vps only if window's dimension change
	if (w != old_w || h != old_h) {
		GLApp::viewportList[0] = { 0, 0, w , h };
		old_w = w;
		old_h = h;
	}

	camera2D.update(GLHelper::ptr_window);

	for (auto & obj : GLApp::objects) {
		if (obj.first != "Camera")obj.second.update(glfwGetTime() - previousTime); // call member function GLObject::draw()
	}

	//Set previosu time as current time
	previousTime = glfwGetTime();
}
/*  _________________________________________________________________________*/
/*! draw

@param none

@return none

 Displays a new title screen with
  - Camera position
  - Orientation
  - no of mystery
  - Window
  - FPS
Sets view port and draw each object
*/
void GLApp::draw() {
	//Get input string stream
	std::stringstream sstr;
	sstr << "Tutorial 3 | Gabe Ng | Camera Position (" << camera2D.pgo->position.x << ',' << camera2D.pgo->position.y
		<< ") | Orientation: " <<static_cast<int>(glm::degrees(camera2D.pgo->orientation.x))<<" degrees"
		<< " | Window height: " << camera2D.height;
	//Get FPS
	double frameTime{ glfwGetTime() - previousTime };
	sstr << " | FPS: " << GLHelper::fps;
	 glfwSetWindowTitle(GLHelper::ptr_window, sstr.str().c_str());
	// Clear the color bit'
	glClear(GL_COLOR_BUFFER_BIT);


	//Set first viewport
	glViewport(viewportList[0].x,viewportList[0].y,viewportList[0].width,viewportList[0].height);

	//if (previousState != GLHelper::keyStateP) {
	//	counter = counter == 2 ?0 :counter + 1;
	//	previousState = GLHelper::keyStateP;
	//}
	//switch (counter) {
	//case 0:
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	break;;
	//case 1:
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	break;;
	//case 2:
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	//	break;;
	//}

	for (auto const& obj : GLApp::objects) {

		if(obj.first!="Camera")obj.second.draw(); // call member function GLObject::draw()
	}
	camera2D.pgo->draw();

}
/*  _________________________________________________________________________*/
/*! cleanup

@param none

@return none

Frees any code used in memory after application exit
*/
void GLApp::cleanup() {
  // empty for now
}


////Set model
//void GLApp::GLModel::setup_vao() {
//
//	//Set position vertex
//	std::array<glm::vec2, 6> pos_vtx{
//	 glm::vec2(0.5f, -0.5f), glm::vec2(0.5f, 0.5f),
//	 glm::vec2(-0.5f, 0.5f), glm::vec2(-0.5f, 0.5f),
//	 glm::vec2(-0.5f, -0.5f), glm::vec2(0.5f, -0.5f)
//	};
//
//
//	GLsizei vertexDataOffset{ 0 };
//	GLsizei vertexAttributeSize{ 0 };
//	GLsizei vertexDataSize{ 0 };
//
//	//Set color vertex
//	std::array<glm::vec3, 6> clr_vtx{
//	 glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f),
//	 glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f),
//	 glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 0.f, 0.f)
//	};
//	//Fill buffer with data
//     vertexDataOffset= 0 ;
//	 vertexAttributeSize= sizeof(glm::vec2);
//	 vertexDataSize=vertexAttributeSize*static_cast<GLsizei>(pos_vtx.size());
//
//	//Fill color buffer with data
//	GLsizei colorDataOffset{ vertexDataSize };
//	GLsizei colorAttributeSize{ sizeof(glm::vec3) };
//	GLsizei colorDataSize{ colorAttributeSize * static_cast<GLsizei>(clr_vtx.size()) };
//
//
//	glCreateBuffers(1, &vboid);
//	glNamedBufferStorage(vboid,vertexDataSize + colorDataSize,nullptr,GL_DYNAMIC_STORAGE_BIT);
//	glNamedBufferSubData(vboid, vertexDataOffset,vertexDataSize, pos_vtx.data());
//	glNamedBufferSubData(vboid, colorDataOffset,colorDataSize, clr_vtx.data());
//	//Allocate spot for vertex array
//	glCreateVertexArrays(1, &vaoid);
//
//	//Bind position attribute to the array index
//	glEnableVertexArrayAttrib(vaoid, 8);
//	glVertexArrayVertexBuffer(vaoid, 3, vboid, vertexDataOffset, vertexAttributeSize);
//	glVertexArrayAttribFormat(vaoid, 8, 2, GL_FLOAT, GL_FALSE, 0);
//	glVertexArrayAttribBinding(vaoid, 8, 3);
//
//	//Binf color attrivute to the array index
//	glEnableVertexArrayAttrib(vaoid, 9);
//	glVertexArrayVertexBuffer(vaoid, 4, vboid, colorDataOffset, colorAttributeSize);
//	glVertexArrayAttribFormat(vaoid, 9, 3, GL_FLOAT, GL_FALSE, 0);
//	glVertexArrayAttribBinding(vaoid, 9, 4);
//
//	//Setting element offsets
//	primitive_type = GL_TRIANGLES;
//	//Bind vertex array to 0
//	glBindVertexArray(0);
//}

//Create shader program
//void GLApp::GLModel::setup_shdrpgm() {
//	if (!shdr_pgm.CompileShaderFromString(GL_VERTEX_SHADER,
//		my_tutorial_2_vs)) {
//		std::cout << "Vertex shader failed to compile: ";
//		std::cout << shdr_pgm.GetLog() << std::endl;
//		std::exit(EXIT_FAILURE);
//	}
//	if (!shdr_pgm.CompileShaderFromString(GL_FRAGMENT_SHADER,
//		my_tutorial_2_fs)) {
//		std::cout << "Fragment shader failed to compile: ";
//		std::cout << shdr_pgm.GetLog() << std::endl;
//		std::exit(EXIT_FAILURE);
//	}
//	if (!shdr_pgm.Link()) {
//		std::cout << "Shader program failed to link!" << std::endl;
//		std::exit(EXIT_FAILURE);
//	}
//	if (!shdr_pgm.Validate()) {
//		std::cout << "Shader program failed to validate!" << std::endl;
//		std::exit(EXIT_FAILURE);
//	}
//}

