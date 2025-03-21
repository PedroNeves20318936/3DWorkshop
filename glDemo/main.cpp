
#include "core.h"
#include "TextureLoader.h"
#include "ArcballCamera.h"
#include "GUClock.h"
//#include "PrincipleAxes.h"
#include "shader_setup.h"
#include "helper.h"
#include "AIMesh.h"
#include "Cube.h"
#include "Scene.h"
#include <FPSCam.h>


using namespace std;
using namespace glm;


#pragma region Global variables

GUClock* g_gameClock = nullptr;

// Mouse tracking
bool				g_mouseDown = false;
double				g_prevMouseX, g_prevMouseY;

// Global Example objects
// shouldn't really be anything in here for the final submission
//ArcballCamera* g_mainCamera = nullptr;
//CGPrincipleAxes* g_principleAxes = nullptr;
Cube* g_cube = nullptr;

GLuint g_flatColourShader;

GLuint g_texDirLightShader;
vec3 g_DLdirection = vec3(0.0f, 1.0f, 0.0f);
vec3 g_DLcolour = vec3(1.0f, 1.0f, 1.0f);
vec3 g_DLambient = vec3(0.2f, 0.2f, 0.2f);

AIMesh* g_creatureMesh = nullptr;
vec3 g_beastPos = vec3(2.0f, 0.0f, 0.0f);
float g_beastRotation = 0.0f;
AIMesh* g_planetMesh = nullptr;
AIMesh* g_duckMesh = nullptr;

//int g_showing = 0;
int g_showingCamera = 0;
int g_NumExamples = 3;

//Global Game Object
Scene* g_Scene = nullptr;

// Window size
const unsigned int g_initWidth = 512;
const unsigned int g_initHeight = 512;

mat4 cameraTransform;
mat4 cameraProjection;
mat4 cameraView;

#pragma endregion


// Function prototypes
void renderScene();
void updateScene();
void resizeWindow(GLFWwindow* _window, int _width, int _height);
void keyboardHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
void mouseMoveHandler(GLFWwindow* _window, double _xpos, double _ypos);
void mouseButtonHandler(GLFWwindow* _window, int _button, int _action, int _mods);
void mouseScrollHandler(GLFWwindow* _window, double _xoffset, double _yoffset);
void mouseEnterHandler(GLFWwindow* _window, int _entered);


int main()
{
	//
	// 1. Initialisation
	//

	g_gameClock = new GUClock();

#pragma region OpenGL and window setup

	// Initialise glfw and setup window
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);

	GLFWwindow* window = glfwCreateWindow(g_initWidth, g_initHeight, "GDV5001", NULL, NULL);

	// Check window was created successfully
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// Set callback functions to handle different events
	glfwSetFramebufferSizeCallback(window, resizeWindow); // resize window callback
	glfwSetKeyCallback(window, keyboardHandler); // Keyboard input callback
	glfwSetCursorPosCallback(window, mouseMoveHandler);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);
	glfwSetScrollCallback(window, mouseScrollHandler);
	glfwSetCursorEnterCallback(window, mouseEnterHandler);

	// Initialise glew
	glewInit();


	// Setup window's initial size
	resizeWindow(window, g_initWidth, g_initHeight);

#pragma endregion

	// Initialise scene - geometry and shaders etc
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // setup background colour to be black
	glClearDepth(1.0f);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//
	// Setup the Example Objects
	//

	g_texDirLightShader = setupShaders(string("Assets\\Shaders\\texture-directional.vert"), string("Assets\\Shaders\\texture-directional.frag"));
	g_flatColourShader = setupShaders(string("Assets\\Shaders\\flatColour.vert"), string("Assets\\Shaders\\flatColour.frag"));

	//g_mainCamera = new ArcballCamera(0.0f, 0.0f, 1.98595f, 55.0f, 1.0f, 0.1f, 500.0f);

	//g_principleAxes = new CGPrincipleAxes();

	g_cube = new Cube();

	g_creatureMesh = new AIMesh(string("Assets\\beast\\beast.obj"));
	if (g_creatureMesh) {
		g_creatureMesh->addTexture(string("Assets\\beast\\beast_texture.bmp"), FIF_BMP);
	}

	g_planetMesh = new AIMesh(string("Assets\\gsphere.obj"));
	if (g_planetMesh) {
		g_planetMesh->addTexture(string("Assets\\Textures\\Hodges_G_MountainRock1.jpg"), FIF_JPEG);
	}

	g_duckMesh = new AIMesh(string("Assets\\duck\\rubber_duck_toy_4k.obj"));
	if (g_duckMesh) {
		g_duckMesh->addTexture(string("Assets\\duck\\rubber_duck_toy_diff_4k.jpg"), FIF_JPEG);
	}

	//
	//Set up Scene class
	//

	g_Scene = new Scene();

	ifstream manifest;
	manifest.open("manifest.txt");

	g_Scene->Load(manifest);
	g_Scene->Init();

	manifest.close();


	//
	// Main loop
	// 

	while (!glfwWindowShouldClose(window))
	{
		updateScene();
		renderScene();						// Render into the current buffer
		glfwSwapBuffers(window);			// Displays what was just rendered (using double buffering).

		glfwPollEvents();					// Use this version when animating as fast as possible

		// update window title
		char timingString[256];
		sprintf_s(timingString, 256, "CIS5013: Average fps: %.0f; Average spf: %f", g_gameClock->averageFPS(), g_gameClock->averageSPF() / 1000.0f);
		glfwSetWindowTitle(window, timingString);
	}

	glfwTerminate();

	if (g_gameClock)
	{
		g_gameClock->stop();
		g_gameClock->reportTimingData();
	}

	return 0;
}


// renderScene - function to render the current scene
void renderScene()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*if (g_Scene)
	{
		Camera* activeCamera = g_Scene->GetActiveCamera();

		if (activeCamera)
		{
			std::string camType = activeCamera->GetType();

			if (camType == "ARCBALLCAMERA")
			{
				ArcballCamera* arcballCam = dynamic_cast<ArcballCamera*>(activeCamera);
				if (arcballCam)
				{
					cameraTransform = arcballCam->projectionTransform() * arcballCam->viewTransform();

					cameraProjection = arcballCam->projectionTransform();
					cameraView = arcballCam->viewTransform() * translate(identity<mat4>(), -g_beastPos);
				}
			}
		}
	}*/


#// Render principle axes - no modelling transforms so just use cameraTransform
	if (true)
	{
		// Render axes 
		/*glUseProgram(g_flatColourShader);
		GLint pLocation;
		Helper::SetUniformLocation(g_flatColourShader, "viewMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraView);
		Helper::SetUniformLocation(g_flatColourShader, "projMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraProjection);
		Helper::SetUniformLocation(g_flatColourShader, "modelMatrix", &pLocation);
		mat4 modelTransform = identity<mat4>();
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);

		g_principleAxes->render();*/
		g_Scene->Render();
	}

	/*switch (g_showing)
	{
	case 0:
	{
		g_Scene->Render();
	}
	break;

	case 1:
	{
		// Render cube 
		glUseProgram(g_flatColourShader);
		GLint pLocation;
		Helper::SetUniformLocation(g_flatColourShader, "viewMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraView);
		Helper::SetUniformLocation(g_flatColourShader, "projMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraProjection);
		Helper::SetUniformLocation(g_flatColourShader, "modelMatrix", &pLocation);
		mat4 modelTransform = glm::translate(identity<mat4>(), vec3(2.0, 0.0, 0.0));
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);

		g_cube->render();
		break;
	}
	case 2:
		glUseProgram(g_texDirLightShader);

		GLint pLocation;
		Helper::SetUniformLocation(g_texDirLightShader, "viewMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraView);
		Helper::SetUniformLocation(g_texDirLightShader, "projMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraProjection);
		Helper::SetUniformLocation(g_texDirLightShader, "texture", &pLocation);
		glUniform1i(pLocation, 0); // set to point to texture unit 0 for AIMeshes
		Helper::SetUniformLocation(g_texDirLightShader, "DIRDir", &pLocation);
		glUniform3fv(pLocation, 1, (GLfloat*)&g_DLdirection);
		Helper::SetUniformLocation(g_texDirLightShader, "DIRCol", &pLocation);
		glUniform3fv(pLocation, 1, (GLfloat*)&g_DLcolour);
		Helper::SetUniformLocation(g_texDirLightShader, "DIRAmb", &pLocation);
		glUniform3fv(pLocation, 1, (GLfloat*)&g_DLambient);
		
		if (g_creatureMesh) {

			// Setup transforms
			Helper::SetUniformLocation(g_texDirLightShader, "modelMatrix", &pLocation);
			mat4 modelTransform = glm::translate(identity<mat4>(), g_beastPos) * eulerAngleY<float>(glm::radians<float>(g_beastRotation));
			glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);

			g_creatureMesh->setupTextures();
			g_creatureMesh->render();
		}

		if (g_planetMesh) {

			// Setup transforms
			Helper::SetUniformLocation(g_texDirLightShader, "modelMatrix", &pLocation);
			mat4 modelTransform = glm::translate(identity<mat4>(), vec3(4.0, 4.0, 4.0));
			glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);

			g_planetMesh->setupTextures();
			g_planetMesh->render();
		}

		if (g_duckMesh) {

			// Setup transforms
			Helper::SetUniformLocation(g_texDirLightShader, "modelMatrix", &pLocation);
			mat4 modelTransform = glm::translate(identity<mat4>(), vec3(0.0, 0.0, 0.0));
			glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);

			g_duckMesh->setupTextures();
			g_duckMesh->render();
		}
		
	}*/

}


// Function called to animate elements in the scene
void updateScene() 
{
	float tDelta = 0.0f;

	if (g_gameClock) {

		g_gameClock->tick();
		tDelta = (float)g_gameClock->gameTimeDelta();
	}

	g_Scene->Update(tDelta);
}


#pragma region Event handler functions
//none of this is currently passed to the Game object
//probably a good idea to do that

// Function to call when window resized
void resizeWindow(GLFWwindow* _window, int _width, int _height)
{
	glViewport(0, 0, _width, _height); // Draw into the entire window

	// Automatically update scene when window is resized
	if (g_Scene) {
		g_Scene->resizeWindowScene(_width, _height);
	}
}



// Function to call to handle keyboard input
void keyboardHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	float moveSpeed = 5.0f;

	if (_action == GLFW_PRESS || _action == GLFW_REPEAT) {
		if (g_Scene) {
			Camera* activeCamera = g_Scene->GetActiveCamera();

			switch (_key) {
			case GLFW_KEY_W:
				activeCamera->moveCamera(0.05f, 0.0f, 0.0f, moveSpeed);
				break;
			case GLFW_KEY_S:
				activeCamera->moveCamera(-0.05f, 0.0f, 0.0f, moveSpeed);
				break;
			case GLFW_KEY_A:
				activeCamera->moveCamera(0.0f, -0.05f, 0.0f, moveSpeed);
				break;
			case GLFW_KEY_D:
				activeCamera->moveCamera(0.0f, 0.05f, 0.0f, moveSpeed);
				break;
			case GLFW_KEY_E:
				activeCamera->moveCamera(0.0f, 0.0f, 0.05f, moveSpeed);
				break;
			case GLFW_KEY_Q:
				activeCamera->moveCamera(0.0f, 0.0f, -0.05f, moveSpeed);
				break;
			}
		}

		switch (_key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(_window, true);
			break;

			/*case GLFW_KEY_SPACE:
				g_showing++;
				g_showing = g_showing % g_NumExamples;
				break;*/

		case GLFW_KEY_C:
			g_showingCamera = (g_showingCamera + 1) % g_Scene->GetCameraCount();
			g_Scene->SwitchCamera(g_showingCamera);
			break;
		case GLFW_KEY_P:
			int width, height;
			glfwGetFramebufferSize(_window, &width, &height);
			g_Scene->resizeWindowScene(width, height);
			break;

		default:
			break;
		}
	}
	else if (_action == GLFW_RELEASE)
	{
		switch (_key)
		{
		default:
			break;
		}
	}
}



void mouseMoveHandler(GLFWwindow* _window, double _xpos, double _ypos)
{
	if (g_Scene)
	{
		Camera* activeCamera = g_Scene->GetActiveCamera();

		if (activeCamera && g_mouseDown)
		{
			float dx = float(_xpos - g_prevMouseX);
			float dy = float(_ypos - g_prevMouseY);

			activeCamera->Rotate(-dy, -dx);

			g_prevMouseX = _xpos;
			g_prevMouseY = _ypos;
		}
	}
}


void mouseButtonHandler(GLFWwindow* _window, int _button, int _action, int _mods) 
{
	if (_button == GLFW_MOUSE_BUTTON_LEFT) 
	{
		if (_action == GLFW_PRESS) 
		{
			g_mouseDown = true;
			glfwGetCursorPos(_window, &g_prevMouseX, &g_prevMouseY);
		}
		else if (_action == GLFW_RELEASE) 
		{
			g_mouseDown = false;
		}
	}
}

void mouseScrollHandler(GLFWwindow* _window, double _xoffset, double _yoffset)
{
	if (g_Scene)
	{
		Camera* activeCamera = g_Scene->GetActiveCamera();

		if (activeCamera)
		{
			if (_yoffset < 0.0)
				activeCamera->Scale(1.1f);
			else if (_yoffset > 0.0)
				activeCamera->Scale(0.9f);
		}
	}
}

void mouseEnterHandler(GLFWwindow* _window, int _entered) 
{
}

#pragma endregion