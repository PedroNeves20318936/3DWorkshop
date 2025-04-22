
#include "OrthographicCamera.h"
#include "stringHelp.h"

using namespace std;
using namespace glm;

//
// Private API
//

// update position, orientation and view matrices when camera rotation and radius is modified
void OrthographicCamera::calculateDerivedValues() {

	const float theta_ = glm::radians<float>(m_theta);
	const float phi_ = glm::radians<float>(m_phi);

	glm::quat yaw = glm::angleAxis(phi_, glm::vec3(0, 1, 0));
	glm::vec3 right = glm::normalize(yaw * glm::vec3(1, 0, 0));
	glm::quat pitch = glm::angleAxis(theta_, right);

	glm::quat camOrientation = pitch * yaw;
	glm::mat4 cameraRotationMatrix = glm::mat4_cast(camOrientation);

	glm::vec3 forward = glm::normalize(cameraRotationMatrix * glm::vec4(0, 0, -1, 0));
	glm::vec3 up = glm::normalize(cameraRotationMatrix * glm::vec4(0, 1, 0, 0));

	m_viewMatrix = glm::lookAt(glm::vec3(m_pos), glm::vec3(m_pos) + forward, up);

	float orthoHeight = 10.0f;
	float orthoWidth = orthoHeight * m_aspect;

	// Here we use an "ortho" projection instead of "perspective" which is the trick to get isometric cameras in OpenGL
	m_projectionMatrix = glm::ortho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, m_nearPlane, m_farPlane);
	//m_projectionMatrix = glm::perspective(glm::radians(m_fovY), m_aspect, m_nearPlane, m_farPlane);
}



//
// Public method implementation
//

// ArcballCamera constructors

// initialise camera parameters so it is placed at the origin looking down the -z axis (for a right-handed camera) or +z axis (for a left-handed camera)
OrthographicCamera::OrthographicCamera() {
	// theta and phi have predefined values to make sure the orthographic camera is slightly tilted (for that isometric look)
	m_theta = -35.0f;
	m_phi = -35.0f;
	m_radius = 15.0f;
	m_pos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_fovY = 55.0f;
	m_aspect = 1.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 500.0f;

	m_type = "ORTHOCAM";
	//F = ViewFrustum(55.0f, 1.0f, 0.1f, 500.0f);

	// calculate derived values
	calculateDerivedValues();
	//F.calculateWorldCoordPlanes(C, R);
}

void OrthographicCamera::Load(ifstream& _file) {
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	StringHelp::Float3(_file, "LOOKAT", m_lookAt.x, m_lookAt.y, m_lookAt.z);
	StringHelp::Float(_file, "FOV", m_fovY);
	StringHelp::Float(_file, "NEAR", m_nearPlane);
	StringHelp::Float(_file, "FAR", m_farPlane);

	calculateDerivedValues();
}


// create a camera with orientation <theta, phi> representing Euler angles specified in degrees and Euclidean distance 'init_radius' from the origin.  The frustum / viewplane projection coefficients are defined in init_fovy, specified in degrees spanning the entire vertical field of view angle, init_aspect (w/h ratio), init_nearPlane and init_farPlane.  If init_farPlane = 0.0 (as determined by equalf) then the resulting frustum represents an infinite perspective projection.  This is the default
OrthographicCamera::OrthographicCamera(float _theta, float _phi, float _radius, float _fovY, float _aspect, float _nearPlane, float _farPlane, glm::vec4 _pos) {

	this->m_theta = _theta;
	this->m_phi = _phi;
	this->m_radius = std::max<float>(0.0f, _radius);
	this->m_pos = _pos;

	this->m_fovY = _fovY;
	this->m_aspect = _aspect;
	this->m_nearPlane = _nearPlane;
	this->m_farPlane = _farPlane;

	//F = ViewFrustum(init_fovy, init_aspect, init_nearPlane, init_farPlane);

	// calculate derived values
	calculateDerivedValues();
	//F.calculateWorldCoordPlanes(C, R);
}


#pragma region Accessor methods for stored values

// return the pivot rotation around the x axis (theta) in degrees
float OrthographicCamera::getTheta() {

	return m_theta;
}

// return the pivot rotation around the y axis (phi) in degrees
float OrthographicCamera::getPhi() {

	return m_phi;
}

void OrthographicCamera::rotateCamera(float _dTheta, float _dPhi) {
	//m_theta += _dTheta;
	m_phi += _dPhi;

	/*const float minTheta = -85.0f;
	const float maxTheta = 85.0f;
	m_theta = glm::clamp(m_theta, minTheta, maxTheta);*/

	calculateDerivedValues();
}



float OrthographicCamera::getRadius() {

	return m_radius;
}

void OrthographicCamera::scaleRadius(float _s) {

	m_radius *= _s;
	calculateDerivedValues();
}

void OrthographicCamera::incrementRadius(float _i) {

	m_radius = std::max<float>(m_radius + _i, 0.0f);
	calculateDerivedValues();
}

float OrthographicCamera::getFovY() {

	return m_fovY;
}

void OrthographicCamera::setFovY(float _fovY) {

	this->m_fovY = _fovY;
	calculateDerivedValues();
}

float OrthographicCamera::getAspect() {

	return m_aspect;
}

void OrthographicCamera::setAspect(float _aspect) {

	this->m_aspect = _aspect;
	calculateDerivedValues();
}

float OrthographicCamera::getNearPlaneDistance() {

	return m_nearPlane;
}

void OrthographicCamera::setNearPlaneDistance(float _nearPlaneDistance) {

	this->m_nearPlane = _nearPlaneDistance;
	calculateDerivedValues();
}

float OrthographicCamera::getFarPlaneDistance() {

	return m_farPlane;
}

void OrthographicCamera::setFarPlaneDistance(float _farPlaneDistance) {

	this->m_farPlane = _farPlaneDistance;
	calculateDerivedValues();
}

#pragma endregion


#pragma region Accessor methods for derived values

// return the camera location in world coordinate space
glm::vec4 OrthographicCamera::getPosition() {

	return m_pos;
}

// return a const reference to the camera's orientation matrix in world coordinate space
//glm::mat4 ArcballCamera::getOrientationBasis() {
//
//	return R;
//}

// return a const reference to the view transform matrix for the camera
glm::mat4 OrthographicCamera::viewTransform() {

	return m_viewMatrix;
}

// return a const reference the projection transform for the camera
glm::mat4 OrthographicCamera::projectionTransform() {

	return m_projectionMatrix;
}

// Function to move my camera you can see that in this specific camera it is missing the "deltaUp" on the movement vec3
// thats because I don't want it to translate on the Y axis (up and down)
void OrthographicCamera::moveCamera(float deltaForward, float deltaRight, float deltaUp, float speed) {

	// First I define the "yaw" based on how much the camera has turned left or right (defined by m_phi)
	glm::quat yaw = glm::angleAxis(glm::radians(m_phi), glm::vec3(0, 1, 0));

	// The "yaw" then is used to figure out the forward (this is so that we can figure out where the cam is facing)
	glm::vec3 forward = glm::normalize(yaw * glm::vec3(0, 0, -1));
	// And the "yaw" is used once more to figure out the right of our camera (basically the same as the "forward" but to figure out where the right of our camera is)
	glm::vec3 right = glm::normalize(yaw * glm::vec3(1, 0, 0));

	// After defining the directions we combine it all into a vec3 
	glm::vec3 movement = (forward * deltaForward + right * deltaRight) * speed;
	// Add this movement into our current position to actually move the camera around
	m_pos += glm::vec4(movement, 0.0f);

	// Print the updated position
	/*std::cout << "Camera Position: X = " << m_pos.x
		<< ", Y = " << m_pos.y
		<< ", Z = " << m_pos.z << std::endl;*/

		// Update the camera's view and projection matrices based on the new position.
	calculateDerivedValues();
}

#pragma endregion