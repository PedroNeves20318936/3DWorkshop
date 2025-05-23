﻿#include "Scene.h"
#include "GameObject.h"
#include "CameraFactory.h"
#include "Camera.h"
#include "LightFactory.h"
#include "Light.h"
#include "ModelFactory.h"
#include "model.h"
#include "Texture.h"
#include "Shader.h"
#include "GameObjectFactory.h"
#include <assert.h>
#include <ArcballCamera.h>
#include <helper.h>
#include <FPSModelCam.h>

Scene::Scene()
{
}

Scene::~Scene()
{
	//TODO: We are being really naught and not deleting everything as we finish
	//what should really go here and in similar places throughout the code base?
}

//tick all my Game Objects, lights and cameras
void Scene::Update(float _dt)
{
	//update all lights
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		(*it)->Tick(_dt);
	}

	//update all cameras
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		(*it)->Tick(_dt);
	}

	//update all GameObjects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Tick(_dt);
	}
}

void Scene::AddGameObject(GameObject* _new)
{
	m_GameObjects.push_back(_new);
}

//I want THAT Game Object by name
GameObject* Scene::GetGameObject(string _GOName)
{
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		if ((*it)->GetName() == _GOName)
		{
			return (*it);
		}
	}
	printf("Unknown Game Object NAME : %s \n", _GOName.c_str());
	assert(0);
	return nullptr;
}

Camera* Scene::GetCamera(string _camName)
{
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		if ((*it)->GetName() == _camName)
		{
			return (*it);
		}
	}
	printf("Unknown Camera NAME : %s \n", _camName.c_str());
	assert(0);
	return nullptr;
}

Light* Scene::GetLight(string _lightName)
{
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		if ((*it)->GetName() == _lightName)
		{
			return (*it);
		}
	}
	printf("Unknown Light NAME : %s \n", _lightName.c_str());
	assert(0);
	return nullptr;
}

Texture* Scene::GetTexture(string _texName)
{
	for (list<Texture*>::iterator it = m_Textures.begin(); it != m_Textures.end(); it++)
	{
		if ((*it)->GetName() == _texName)
		{
			return (*it);
		}
	}
	printf("Unknown Texture NAME : %s \n", _texName.c_str());
	assert(0);
	return nullptr;
}

Model* Scene::GetModel(string _modelName)
{
	for (list<Model*>::iterator it = m_Models.begin(); it != m_Models.end(); it++)
	{
		if ((*it)->GetName() == _modelName)
		{
			return (*it);
		}
	}
	printf("Unknown Model NAME : %s \n", _modelName.c_str());
	assert(0);
	return nullptr;
}

Shader* Scene::GetShader(string _shaderName)
{
	for (list<Shader*>::iterator it = m_Shaders.begin(); it != m_Shaders.end(); it++)
	{
		if ((*it)->GetName() == _shaderName)
		{
			return (*it);
		}
	}
	printf("Unknown Shader NAME : %s \n", _shaderName.c_str());
	assert(0);
	return nullptr;
}


void Scene::Render()
{
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		// this section makes sure that the "BEAST" is only rendered if the user is not on the "FPSMODELCAMERA" (to avoid clipping while on first person)
		if (m_useCamera && m_useCamera->GetName() == "FPSMODELCAMERA" && (*it)->GetName() == "BEAST")
			continue;
		// this section makes sure that the "ARMS" is only rendered if the user is on the "FPSMODELCAMERA" (to avoid arm duplication)
		if (m_useCamera && m_useCamera->GetName() != "FPSMODELCAMERA" && (*it)->GetName() == "ARMS")
			continue;
		// this section makes sure that the "CEILING" is only rendered if the user is on the "FPSMODELCAMERA" (to avoid blocking the scene for other cameras)
		if (m_useCamera && m_useCamera->GetName() != "FPSMODELCAMERA" && (*it)->GetName() == "CEILING")
			continue;

		if ((*it)->GetRP() & RP_OPAQUE)
		{
			GLuint SP = (*it)->GetShaderProg();
			glUseProgram(SP);

			m_useCamera->SetRenderValues(SP);
			SetShaderUniforms(SP);

			glm::mat4 projectionMatrix = m_useCamera->GetProjectionMatrix();
			glm::mat4 viewMatrix = m_useCamera->GetViewMatrix();

			GLint pLocation;
			Helper::SetUniformLocation(SP, "viewMatrix", &pLocation);
			glUniformMatrix4fv(pLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

			Helper::SetUniformLocation(SP, "projMatrix", &pLocation);
			glUniformMatrix4fv(pLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

			(*it)->PreRender();
			(*it)->Render();
		}
	}

	// the following for loop is responsible for rendering transparent/translucent objects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		// it first then checks if the object has the "RP_TRANSPARENT" render pass assigned
		if ((*it)->GetRP() & RP_TRANSPARENT)
		{
			// enables blending on OpenGL (this allows for the transparency to be drawn correctly)
			glEnable(GL_BLEND);
			// makes transparent pixels blend with the background (depending on their alpha values)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			// disables writing on the depth buffer (so that transparent objects don't block other transparent objects)
			glDepthMask(GL_FALSE);

			GLuint SP = (*it)->GetShaderProg();
			glUseProgram(SP);

			m_useCamera->SetRenderValues(SP);
			SetShaderUniforms(SP);

			glm::mat4 projectionMatrix = m_useCamera->GetProjectionMatrix();
			glm::mat4 viewMatrix = m_useCamera->GetViewMatrix();

			GLint pLocation;
			Helper::SetUniformLocation(SP, "viewMatrix", &pLocation);
			glUniformMatrix4fv(pLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

			Helper::SetUniformLocation(SP, "projMatrix", &pLocation);
			glUniformMatrix4fv(pLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

			(*it)->PreRender();
			(*it)->Render();

			// reverts settings to normal
			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ZERO);
		}

	}
}

	// Your RP_TRANSPARENT logic here (

void Scene::SetShaderUniforms(GLuint _shaderprog)
{
	//everything needs to know about all the lights
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		(*it)->SetRenderValues(_shaderprog);
	}

}

void Scene::Load(ifstream& _file)
{
	string dummy;

	//load Cameras
	_file >> dummy >> m_numCameras; _file.ignore(256, '\n');
	cout << "CAMERAS : " << m_numCameras << endl;
	for (int i = 0; i < m_numCameras; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Camera* newCam = CameraFactory::makeNewCam(type);
		newCam->Load(_file);

		m_Cameras.push_back(newCam);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Lights
	_file >> dummy >> m_numLights; _file.ignore(256, '\n');
	cout << "LIGHTS : " << m_numLights << endl;
	for (int i = 0; i < m_numLights; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Light* newLight = LightFactory::makeNewLight(type);
		newLight->Load(_file);

		m_Lights.push_back(newLight);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Models
	_file >> dummy >> m_numModels; _file.ignore(256, '\n');
	cout << "MODELS : " << m_numModels << endl;
	for (int i = 0; i < m_numModels; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Model* newModel = ModelFactory::makeNewModel(type);
		newModel->Load(_file);

		m_Models.push_back(newModel);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Textures
	_file >> dummy >> m_numTextures; _file.ignore(256, '\n');
	cout << "TEXTURES : " << m_numTextures << endl;
	for (int i = 0; i < m_numTextures; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		m_Textures.push_back(new Texture(_file));

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Shaders
	_file >> dummy >> m_numShaders; _file.ignore(256, '\n');
	cout << "SHADERS : " << m_numShaders << endl;
	for (int i = 0; i < m_numShaders; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		m_Shaders.push_back(new Shader(_file));

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load GameObjects
	_file >> dummy >> m_numGameObjects; _file.ignore(256, '\n');
	cout << "GAMEOBJECTS : " << m_numGameObjects << endl;
	for (int i = 0; i < m_numGameObjects; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		GameObject* newGO = GameObjectFactory::makeNewGO(type);
		newGO->Load(_file);

		m_GameObjects.push_back(newGO);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	// This section gets the fps camera by its name
	FPSModelCam* fpsModelCam = dynamic_cast<FPSModelCam*>(GetCamera("FPSMODELCAMERA"));

	// These following lines get the beast models (both the full version and the arms only version)
	GameObject* arms = GetGameObject("ARMS");
	GameObject* beast = GetGameObject("BEAST");

	// It then attaches these models into the camera to create a "possession" effect
	// I also call for "ForceUpdate" just to make sure everything is updated right as the game starts
	if (fpsModelCam && arms && beast) 
	{
		fpsModelCam->AttachArms(arms);
		fpsModelCam->AttachBeast(beast);
		fpsModelCam->ForceUpdate();
	}
}

void Scene::Init()
{
	//initialize all cameras
	//scene is passed down here to allow for linking of cameras to game objects
	int count = 0;
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
	{
		(*it)->Init(100, 100, this);// TODO: set correct screen sizes here

		//if a camera is called MAIN
		//this will be the starting camera used
		if ((*it)->GetName() == "MAIN")
		{
			m_useCamera = (*it);
			m_useCameraIndex = count;
		}
		count++;
	}

	//if no MAIN camera just use the first one
	if (!m_useCamera)
	{
		m_useCamera = (*m_Cameras.begin());
		m_useCameraIndex = 0;
	}

	//set up links between everything and GameObjects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Init(this);
	}
}
 
void Scene::SwitchCamera(int cameraIndex)
{
	// wraps the index around (cant go out of bonds)
	cameraIndex = cameraIndex % m_Cameras.size();

	// "it" is an iterator that is assigned to the beginning of my camera list
	auto it = m_Cameras.begin();
	// advance the iterator to the specified index (selects the desired camera)
	std::advance(it, cameraIndex);

	// set use camera to "it", therefore switching to the next camera
	m_useCamera = *it;
	cout << "Switching to: " << m_useCamera->GetName() << endl;
}

void Scene::resizeWindowScene(int _width, int _height)
{
	if (m_Cameras.empty())
	{
		cout << "No cameras available!" << endl;
		return;
	}

	if (_height == 0) _height = 1;

	float newAspect = static_cast<float>(_width) / static_cast<float>(_height);
	//cout << "Resizing window! New aspect: " << newAspect << " (Width: " << _width << ", Height: " << _height << ")" << endl;

	for (Camera* cam : m_Cameras)
	{
		cam->UpdateAspectRatio(newAspect);
	}

	glViewport(0, 0, _width, _height);
}

Camera* Scene::GetActiveCamera()
{
	return m_useCamera;
}

