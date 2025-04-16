/* !
@file    	  glhelper.h
@author  	  pghali@digipen.edu
@co-author	  parminder.singh@digipen.edu , gabe.ng@digipen.edu
@date    	  04/06/2024

This file contains the declaration of namespace Helper that encapsulates the
functionality required to create an OpenGL context using GLFW; use GLEW
to load OpenGL extensions; initialize OpenGL state; and finally initialize
the OpenGL application by calling initalization functions associated with
objects participating in the application.

*//*__________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GLHELPER_H
#define GLHELPER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <string>
#include <glslshader.h>
/*  _________________________________________________________________________ */
struct GLHelper
  /*! GLHelper structure to encapsulate initialization stuff ...
  */
{
  static bool init(GLint width, GLint height, std::string title);
  static void cleanup();

  // callbacks ...
  static void error_cb(int error, char const* description);
  static void fbsize_cb(GLFWwindow *ptr_win, int width, int height);
  // I/O callbacks ...
  static void key_cb(GLFWwindow *pwin, int key, int scancode, int action, int mod);
  static void mousebutton_cb(GLFWwindow *pwin, int button, int action, int mod);
  static void mousescroll_cb(GLFWwindow *pwin, double xoffset, double yoffset);
  static void mousepos_cb(GLFWwindow *pwin, double xpos, double ypos);
  // setup callbacks for events
  static void setup_event_callbacks();

  static void update_time(double fpsCalcInt = 1.0);

  //Print ddetails
  /*  _________________________________________________________________________ */
/*! print_specs

@param none

@return none

Pritns specifiications used by the computer in the command window
Prints
    GPU Vendor
    GL renderer
    GL Version
    GL Shader version
    GL Major and Minor version
    Current OpenGL double buffer
    Maximum vertice count
    Maximum Indices count
    Max texture size
    Viewport Dimensions
    Vertex attributes
    Vertex buffer bindings
*/
  static void print_specs();

  static GLint width, height;
  static GLdouble fps;
  static GLdouble delta_time; // time taken to complete most recent game loop
  static std::string title;
  static GLFWwindow *ptr_window;

  //Key events
  static GLboolean keyStateV,keyStateH,keyStateK, keyStateU, keyStateZ;

  static GLint renderAmt;

  static std::string shaderName;

};
/*  _________________________________________________________________________*/
/*! operator<<

@parm std::ostream&
Reference to the left hand side output stream

@parm glm::vec2
The data type to overload 

@return none

This function makes it easier to debug, allowing you to use the insert 
operator on a vec 2 and have it output its respective values

*/
std::ostream& operator<< (std::ostream& os, glm::vec2 vec2);
#endif /* GLHELPER_H */
