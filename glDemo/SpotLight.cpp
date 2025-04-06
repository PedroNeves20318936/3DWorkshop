#include "core.h"
#include "SpotLight.h"
#include "helper.h"
#include "stringHelp.h"

SpotLight::SpotLight()
{
	m_type = "SPOT";
	m_pos = vec3(0.0, 0.0, 0.0);
	m_direction = vec3(0.0, -1.0, 0.0);
	m_cutoffAngle = glm::cos(glm::radians(12.5f));
	m_cutoffOuterAngle = glm::cos(glm::radians(0.6f));
}

SpotLight::~SpotLight()
{
}

void SpotLight::Load(ifstream& _file)
{
	Light::Load(_file);
	StringHelp::Float3(_file, "SPOT", m_direction.x, m_direction.y, m_direction.z);

	float cutoffDegrees;
	StringHelp::Float(_file, "CUTOFF", cutoffDegrees);
	m_cutoffAngle = glm::cos(glm::radians(cutoffDegrees));

	float cutoffOuterDegrees;
	StringHelp::Float(_file, "OUTERCUTOFF", cutoffOuterDegrees);
	m_cutoffOuterAngle = glm::cos(glm::radians(cutoffOuterDegrees));
}

void SpotLight::SetRenderValues(unsigned int _prog)
{
	//still need to tell the shader about the basic light data
	Light::SetRenderValues(_prog);

	GLint loc;
	string dirString = m_name + "Dir";//only thing I add is a direction

	if (Helper::SetUniformLocation(_prog, dirString.c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(m_direction));

	string cutString = m_name + "Cutoff";

	if (Helper::SetUniformLocation(_prog, cutString.c_str(), &loc))
		glUniform1f(loc, m_cutoffAngle);

	string posString = m_name + "Pos";
	if (Helper::SetUniformLocation(_prog, posString.c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(m_pos));

	string outCutString = m_name + "OuterCutoff";

	if (Helper::SetUniformLocation(_prog, outCutString.c_str(), &loc))
		glUniform1f(loc, m_cutoffOuterAngle);

}
