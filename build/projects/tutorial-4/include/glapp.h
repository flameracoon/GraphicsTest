/* !
@file    	glapp.h
@author  	pghali@digipen.edu
@coauthor	parminder.singh@digipen.edu , gabe.ng@digipen.edu
@date    	  04/06/2024

This file contains the declaration of namespace GLApp that encapsulates the
functionality required to implement an OpenGL application including
compiling, linking, and validating shader programs
setting up geometry and index buffers,
configuring VAO to present the buffered geometry and index data to
vertex shaders,
configuring textures (in later labs),
configuring cameras (in later labs),
and transformations (in later labs).
*//*__________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GLAPP_H
#define GLAPP_H
#include <glhelper.h>
#include <glslshader.h>
#include <array>
#include <vector>
#include <map>
#include <fstream>
/*                                                                   includes
----------------------------------------------------------------------------- */

struct GLApp {
	/*  _________________________________________________________________________*/
	/*! init

	@param none

	@return none

	 Initializes the random seed generator and
	 starting viewport settings

	 Sets the clear color of the background

	 Initializes the objects, models and shaders in scene via InitScene
	 Afterwich it initializes all the objects as well as the starting camera object

	*/
  static void init();
  /*  _________________________________________________________________________*/
  /*! update

  @param none

  @return none

   Updates the viewport for it to be resized
   Updates the camera based on inputs
   Loops through each object and updates everyone except camera
  */
  static void update();
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
  static void draw();
  /*  _________________________________________________________________________*/
  /*! cleanup

  @param none

  @return none

  Frees any code used in memory after application exit
  */
  static void cleanup();

  struct GLModel {
	  //Settings
	  GLenum primitive_type{GL_TRIANGLES};
	  GLuint primitiveCount{ 0 };
	  GLuint vaoid{0};
	  GLuint drawCount{ 0 };
/*  _________________________________________________________________________*/
/*! init

@parm std::string
The mesh file to be read from

Reads from the mesh file and saves data in it into the GLModel

*/
	  void init(std::string meshFile);

  };
  struct GLObject {
	  glm::vec2 orientation;
	  glm::vec2 scaling{1.f,1.f};
	  glm::vec2 position{0.f,0.f};

	  glm::mat3 mdlToNDC{1.f};
	  std::map<std::string, GLApp::GLModel>::iterator modelRef;
	  std::map<std::string, GLSLShader>::iterator shaderRef;

	  glm::vec3 color;
	  glm::mat3 mdl;
/*  _________________________________________________________________________*/
/*! init

@param none

@return none

Initializes GLObject's Model to NDC matrix
	This tabulates the concat between the scale, rotation and translation first
	before concating with the camera's world to NDC matrix to 
	derive the new NDC matrix
*/
	  void init();
/*  _________________________________________________________________________*/
/*! update

@parm GLdouble
takes in the deltatime to be used with rotation

@return none

Tabulates the new Model to NDC matrix after the object rotated X amount
of degrees

*/
	  void update(GLdouble deltaTime);
/*  _________________________________________________________________________*/
/*! draw

@param none

@return none

Renders the object
Uses the shader reference and model reference to render
Model to NDC matrix is set, followed by the color vector 

Array and shaders are unbinded after use
*/
	  void draw() const;
  };

  static std::map<std::string, GLObject> objects;  
  //List of models
  static std::map<std::string, GLModel> models;
  //Set up new models
  static std::map<std::string, GLSLShader>shaderPrograms;
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
  static void InsertShaderPgm(std::string, std::string, std::string);
 
  /*  _________________________________________________________________________*/
/*! InitScene

@parm std::string
The scene file to be read from

Reads the scene file and generates objects/ models based on it

*/
  static void InitScene(std::string);
  //Set viewport structure
  struct GLViewport {
	  GLint x, y;
	  GLsizei width, height;
  };
  //Create a vecor of viewports
  static std::vector <GLViewport>viewportList;

  struct Camera2D {
	  GLObject* pgo;
	  glm::vec2 right, up;
	  glm::mat3 viewMat{ 1.0f };

	  GLint height{ 1000 };
	  GLfloat ar;
	 // window change parameters ...
	  GLint min_height{ 500 }, max_height{ 2000 };
	  // height is increasing if 1 and decreasing if -1
	  GLint heightChangeDir{ 1 };
	  // increments by which window height is changed per Z key press
	  GLint heightChangeVal{ 5 };

	  // camera's speed when button U is pressed
	  GLfloat linear_speed{ 2.f };
	  glm::mat3 camwinToNDC{1.f};
	  glm::mat3 worldToNDC{1.f};
 /*  _________________________________________________________________________*/
/*! init

@parm GLFWwindow*
The window refernced by the camera

@parm GLObject*
The camera object

@return none

Initializes the camera object with the relevant oritentation, position and matrix

*/
	  void init(GLFWwindow* wind, GLObject* ptr);
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
	  void update(GLFWwindow*);
  };
  static Camera2D camera2D;
};

#endif /* GLAPP_H */
