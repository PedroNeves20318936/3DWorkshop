#include "Camera.h"
#include "helper.h"
#include <fstream>
#include <iostream>
#include "stringHelp.h"

using namespace std;
using namespace glm;

/////////////////////////////////////////////////////////////////////////////////////
// constructor
/////////////////////////////////////////////////////////////////////////////////////
Camera::Camera()
{
	m_theta = 0.0f;
	m_phi = 0.0f;
	m_radius = 15.0f;

	m_fovY = 55.0f;
	m_aspect = 1.0f;
	m_near = 0.1f;
	m_far = 500.0f;

	//F = ViewFrustum(55.0f, 1.0f, 0.1f, 500.0f);

	// calculate derived values
	calculateDerivedValues();
	//F.calculateWorldCoordPlanes(C, R);

	m_type = "CAMERA";
}

/////////////////////////////////////////////////////////////////////////////////////
// destructor
/////////////////////////////////////////////////////////////////////////////////////
Camera::~Camera()
{
}

/////////////////////////////////////////////////////////////////////////////////////
// Init() - 
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Init(float _screenWidth, float _screenHeight, Scene* _scene)
{
	//TODO: move the calculation of the Projection Matrix to Camera::Tick
	// so that we can do the same rescaling of the aspect ratio to match the current window
	float aspect_ratio = _screenWidth / _screenHeight;
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), aspect_ratio, m_near, m_far);
}

/////////////////////////////////////////////////////////////////////////////////////
// Update() - 
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Tick(float _dt)
{
	m_viewMatrix = glm::lookAt(m_pos, m_lookAt, vec3(0, 1, 0));
}

void Camera::Load(ifstream& _file)
{
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	StringHelp::Float3(_file, "LOOKAT", m_lookAt.x, m_lookAt.y, m_lookAt.z);
	StringHelp::Float(_file, "FOV", m_fov);
	StringHelp::Float(_file, "NEAR", m_near);
	StringHelp::Float(_file, "FAR", m_far);
}

//set the base render values for this camera in the shaders
void Camera::SetRenderValues(unsigned int _prog)
{
	GLint loc;

	//matrix for the view transform
	if (Helper::SetUniformLocation(_prog, "viewMatrix", &loc))
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(GetView()));

	//matrix for the projection transform
	if (Helper::SetUniformLocation(_prog, "projMatrix", &loc))
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(GetProj()));

	//the current camera is at this position
	if (Helper::SetUniformLocation(_prog, "camPos", &loc))
		glUniform3fv(loc, 1, glm::value_ptr(GetPos()));
}

float Camera::getAspect() {

	return m_aspect;
}

void Camera::setAspect(float _aspect) {

	this->m_aspect = _aspect;
	calculateDerivedValues();
}

void Camera::calculateDerivedValues()
{
	const float theta_ = glm::radians<float>(m_theta);
	const float phi_ = glm::radians<float>(m_phi);

	m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_radius)) * glm::eulerAngleX(-theta_) * glm::eulerAngleY(-phi_);
	m_projectionMatrix = glm::perspective(glm::radians<float>(m_fovY), m_aspect, m_near, m_far);
}
