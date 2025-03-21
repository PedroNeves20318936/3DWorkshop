#include "CameraFactory.h"
#include "Camera.h"
#include "ArcballCamera.h"
#include "FPSCam.h"
#include "OrthographicCamera.h"
#include <assert.h>

using std::string;

Camera* CameraFactory::makeNewCam(string _type)
{
	printf("CAM TYPE: %s \n", _type.c_str());
	if (_type == "CAMERA")
	{
		return new Camera();
	}
	else if (_type == "ARCCAMERA")
	{
		return new ArcballCamera();
	}
	else if (_type == "FPSCAMERA")
	{
		return new FPSCam();
	}
	else if (_type == "ORTHOCAM")
	{
		return new OrthographicCamera();
	}
	else
	{
		printf("UNKNOWN CAMERA TYPE!");
		assert(0);
		return nullptr;
	}
}
