/*!
@file    	  glhelper.cpp
@author  	  pghali@digipen.edu
@co-author	  parminder.singh@digipen.edu , gabe.ng@digipen.edu
@date    	  04/06/2024

This file implements functionality useful and necessary to build OpenGL
applications including use of external APIs such as GLFW to create a
window and start up an OpenGL context and use GLEW to extract function
pointers to OpenGL implementations.

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <glhelper.h>
#include <iostream>
#include <hook_csd2101.h>

/*                                                   objects with file scope
----------------------------------------------------------------------------- */
// static data members declared in GLHelper
GLint GLHelper::width;
GLint GLHelper::height;
GLdouble GLHelper::fps;
GLdouble GLHelper::delta_time;
std::string GLHelper::title;
GLFWwindow* GLHelper::ptr_window;
GLboolean GLHelper::keyStateV{ false }, GLHelper::keyStateH{ false }, GLHelper::keyStateK{ false }, GLHelper::keyStateU{ false }, GLHelper::keyStateZ{ false };
GLint GLHelper::renderAmt{0};

std::string GLHelper::shaderName{ "DEFAULT" };

bool switchCon{false};
/*  _________________________________________________________________________ */
/*! init

@param GLint width
@param GLint height
Dimensions of window requested by program

@param std::string title_str
String printed to window's title bar

@return bool
true if OpenGL context and GLEW were successfully initialized.
false otherwise.

Uses GLFW to create OpenGL context. GLFW's initialization follows from here:
http://www.glfw.org/docs/latest/quick.html
a window of size width x height pixels
and its associated OpenGL context that matches a core profile that is 
compatible with OpenGL 4.5 and doesn't support "old" OpenGL, has 32-bit RGBA,
double-buffered color buffer, 24-bit depth buffer and 8-bit stencil buffer 
with each buffer of size width x height pixels
*/
bool GLHelper::init(GLint width, GLint height, std::string title) {
  GLHelper::width = width;
  GLHelper::height = height;
  GLHelper::title = title;

  // Part 1
  if (!glfwInit()) {
    std::cout << "GLFW init has failed - abort program!!!" << std::endl;
    return false;
  }

  // In case a GLFW function fails, an error is reported to callback function
  glfwSetErrorCallback(GLHelper::error_cb);

  // Before asking GLFW to create an OpenGL context, we specify the minimum constraints
  // in that context:
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);

  GLHelper::ptr_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!GLHelper::ptr_window) {
    std::cerr << "GLFW unable to create OpenGL context - abort program\n";
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(GLHelper::ptr_window);
  
  setup_event_callbacks();

  // this is the default setting ...
  glfwSetInputMode(GLHelper::ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // Part 2: Initialize entry points to OpenGL functions and extensions
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cerr << "Unable to initialize GLEW - error: "
      << glewGetErrorString(err) << " abort program" << std::endl;
    return false;
  }
  if (GLEW_VERSION_4_5) {
    std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
  } else {
    std::cerr << "Warning: The driver may lack full compatibility with OpenGL 4.5, potentially limiting access to advanced features." << std::endl;
  }

  return true;
}

void GLHelper::setup_event_callbacks() {
    AUTOMATION_HOOK_EVENTS(); // Automation hook. [!WARNING!] Do not alter/remove this! 
    
    glfwSetFramebufferSizeCallback(GLHelper::ptr_window, GLHelper::fbsize_cb);
    glfwSetKeyCallback(GLHelper::ptr_window, GLHelper::key_cb);
    glfwSetMouseButtonCallback(GLHelper::ptr_window, GLHelper::mousebutton_cb);
    glfwSetCursorPosCallback(GLHelper::ptr_window, GLHelper::mousepos_cb);
    glfwSetScrollCallback(GLHelper::ptr_window, GLHelper::mousescroll_cb);
}

/*  _________________________________________________________________________ */
/*! cleanup

@param none

@return none

For now, there are no resources allocated by the application program.
The only task is to have GLFW return resources back to the system and
gracefully terminate.
*/
void GLHelper::cleanup() {
  // Part 1
  glfwTerminate();
}

/*  _________________________________________________________________________*/
/*! key_cb

@param GLFWwindow*
Handle to window that is receiving event

@param int
the keyboard key that was pressed or released

@parm int
Platform-specific scancode of the key

@parm int
GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
action will be GLFW_KEY_UNKNOWN if GLFW lacks a key token for it,
for example E-mail and Play keys.

@parm int
bit-field describing which modifier keys (shift, alt, control)
were held down

@return none

This function is called when keyboard buttons are pressed.
When the ESC key is pressed, the close flag of the window is set.

Presing V toggles between two states for the camera, one is a free camera while the other takes the form of a first
person camera
Pressing H increaces the angle and pressing K decreases it
Pressing U moves it in the direction the triangle is facing
Pressing Z zooms it in/out


*/
void GLHelper::key_cb(GLFWwindow *pwin, int key, int scancode, int action, int mod) {
  if (GLFW_PRESS == action) {
#ifdef _DEBUG
    std::cout << "Key pressed" << std::endl;
#endif

    switch (key) {
        case GLFW_KEY_V:
            keyStateV = keyStateV == GL_TRUE ? GL_FALSE : GL_TRUE;
        break;;
        case GLFW_KEY_H:
            keyStateH = GL_TRUE;
            break;;
        case GLFW_KEY_K:
            keyStateK = GL_TRUE;
            break;
        case GLFW_KEY_U:
            keyStateU = GL_TRUE;
            break;;
        case GLFW_KEY_Z:
            keyStateZ = GL_TRUE;
            break;;
        break;;
        case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(pwin, GLFW_TRUE);
        break;;
        
    }


  } else if (GLFW_REPEAT == action) {
#ifdef _DEBUG
    std::cout << "Key repeatedly pressed" << std::endl;
#endif
  } else if (GLFW_RELEASE == action) {
#ifdef _DEBUG
    std::cout << "Key released" << std::endl;
#endif
    switch (key) {

        case GLFW_KEY_H:
            keyStateH = GL_FALSE;
            break;;
        case GLFW_KEY_K:
            keyStateK = GL_FALSE;
            break;
        case GLFW_KEY_U:
            keyStateU = GL_FALSE;
            break;;
        case GLFW_KEY_Z:
            keyStateZ = GL_FALSE;
            break;;

    }
  }

}

/*  _________________________________________________________________________*/
/*! mousebutton_cb

@param GLFWwindow*
Handle to window that is receiving event

@param int
the mouse button that was pressed or released
GLFW_MOUSE_BUTTON_LEFT and GLFW_MOUSE_BUTTON_RIGHT specifying left and right
mouse buttons are most useful

@parm int
action is either GLFW_PRESS or GLFW_RELEASE

@parm int
bit-field describing which modifier keys (shift, alt, control)
were held down

@return none

*/
void GLHelper::mousebutton_cb(GLFWwindow *pwin, int button, int action, int mod) {
  switch (button) {
  case GLFW_MOUSE_BUTTON_LEFT:
#ifdef _DEBUG
    std::cout << "Left mouse button ";
#endif
   
  
    break;
  case GLFW_MOUSE_BUTTON_RIGHT:
#ifdef _DEBUG
    std::cout << "Right mouse button ";
#endif
    break;
  }
  switch (action) {
  case GLFW_PRESS:
#ifdef _DEBUG
    std::cout << "pressed!!!" << std::endl;
#endif

    break;
  case GLFW_RELEASE:
#ifdef _DEBUG
    std::cout << "released!!!" << std::endl;
#endif
    break;
  }
}

/*  _________________________________________________________________________*/
/*! mousepos_cb

@param GLFWwindow*
Handle to window that is receiving event

@param double
new cursor x-coordinate, relative to the left edge of the client area

@param double
new cursor y-coordinate, relative to the top edge of the client area

@return none

This functions receives the cursor position, measured in screen coordinates but
relative to the top-left corner of the window client area.
*/
void GLHelper::mousepos_cb(GLFWwindow *pwin, double xpos, double ypos) {
#ifdef _DEBUG
  std::cout << "Mouse cursor position: (" << xpos << ", " << ypos << ")" << std::endl;
#endif
}

/*  _________________________________________________________________________*/
/*! mousescroll_cb

@param GLFWwindow*
Handle to window that is receiving event

@param double
Scroll offset along X-axis

@param double
Scroll offset along Y-axis

@return none

This function is called when the user scrolls, whether with a mouse wheel or
touchpad gesture. Although the function receives 2D scroll offsets, a simple
mouse scroll wheel, being vertical, provides offsets only along the Y-axis.
*/
void GLHelper::mousescroll_cb(GLFWwindow *pwin, double xoffset, double yoffset) {
#ifdef _DEBUG
  std::cout << "Mouse scroll wheel offset: ("
    << xoffset << ", " << yoffset << ")" << std::endl;
#endif
 
}

/*  _________________________________________________________________________ */
/*! error_cb

@param int
GLFW error code

@parm char const*
Human-readable description of the code

@return none

The error callback receives a human-readable description of the error and
(when possible) its cause.
*/
void GLHelper::error_cb(int error, char const* description) {
#ifdef _DEBUG
  std::cerr << "GLFW error: " << description << std::endl;
#endif
}

/*  _________________________________________________________________________ */
/*! fbsize_cb

@param GLFWwindow*
Handle to window that is being resized

@parm int
Width in pixels of new window size

@parm int
Height in pixels of new window size

@return none

This function is called when the window is resized - it receives the new size
of the window in pixels.
*/
void GLHelper::fbsize_cb(GLFWwindow *ptr_win, int width, int height) {
#ifdef _DEBUG
  std::cout << "fbsize_cb getting called!!!" << std::endl;
#endif
  GLHelper::width = width;
  GLHelper::height = height;
}

/*  _________________________________________________________________________*/
/*! update_time

@param double
fps_calc_interval: the interval (in seconds) at which fps is to be
calculated

This function must be called once per game loop. It uses GLFW's time functions
to compute:
1. the interval in seconds between each frame
2. the frames per second every "fps_calc_interval" seconds
*/
void GLHelper::update_time(double fps_calc_interval) {
  // get elapsed time (in seconds) between previous and current frames
  static double prev_time = glfwGetTime();
  double curr_time = glfwGetTime();
  delta_time = curr_time - prev_time;
  prev_time = curr_time;

  // fps calculations
  static double count = 0.0; // number of game loop iterations
  static double start_time = glfwGetTime();
  // get elapsed time since very beginning (in seconds) ...
  double elapsed_time = curr_time - start_time;

  ++count;

  // update fps at least every 10 seconds ...
  fps_calc_interval = (fps_calc_interval < 0.0) ? 0.0 : fps_calc_interval;
  fps_calc_interval = (fps_calc_interval > 10.0) ? 10.0 : fps_calc_interval;
  if (elapsed_time > fps_calc_interval) {
    GLHelper::fps = count / elapsed_time;
    start_time = curr_time;
    count = 0.0;
  }
}
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
void GLHelper::print_specs() {
   // std::cout << "TESTING PRINT SPECS";
    //Retrieve versions with OpenGL get string
    GLubyte const* strList[4];
    strList[0] = glGetString(GL_VENDOR);
    strList[1] = glGetString(GL_RENDERER);
    strList[2] = glGetString(GL_VERSION);
    strList[3] = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "GPU Vendor: " << strList[0] << std::endl;
    std::cout << "GL Renderer: " << strList[1] << std::endl;
    std::cout << "GL Version: " << strList[2] << std::endl;
    std::cout << "GL Shader Version: " << strList[3] << std::endl;
    //Retrieve versions with open gl integer
    GLint intList[9];
    glGetIntegerv(GL_MAJOR_VERSION, &intList[0]);   
    glGetIntegerv(GL_MINOR_VERSION, &intList[1]);
    glGetIntegerv(GL_DOUBLEBUFFER, &intList[2]);
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &intList[3]);
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &intList[4]);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &intList[5]);
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &intList[6]);
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &intList[7]);
    glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &intList[8]);

    std::cout << "GL Major Version: " << intList[0] << std::endl;
    std::cout << "GL Minor Version: " << intList[1] << std::endl;
   if(intList[2]) std::cout << "Current OpenGL Context is double-buffered"<< std::endl;else  std::cout << "Current OpenGL Context is not double-buffered" << std::endl;
   std::cout << "Maximum Vertices Count: " << intList[3] << std::endl;
   std::cout << "Maximum Indices Count: " << intList[4] << std::endl;
   std::cout << "GL Maximum texture size " << intList[5] << std::endl;
   std::cout << "Maximum Viewport Dimensions:  " << intList[6] << std::endl;
   std::cout << "Maximum generic vertex attributes  " << intList[7] << std::endl;
   std::cout << "Maximum vertex buffer bindings  " << intList[8] << std::endl;
}
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
std::ostream& operator<< (std::ostream& os, glm::vec2 vec2) {
    os << vec2.x;
    os << ' ' << vec2.y;
    return os;
}