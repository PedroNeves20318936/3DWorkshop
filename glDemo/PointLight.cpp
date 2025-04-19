#include "core.h"
#include "PointLight.h"
#include "helper.h"
#include "stringHelp.h"

PointLight::PointLight()
{
	m_type = "POINT";
	m_pos = vec3(0.0, 0.0, 0.0);
	m_constant = 1.0f;
	m_linear = 0.09f;
	m_quadratic = 0.032f;
}

PointLight::~PointLight() {}

void PointLight::Load(std::ifstream& _file)
{
	Light::Load(_file);
	StringHelp::Float(_file, "CONSTANT", m_constant);
	StringHelp::Float(_file, "LINEAR", m_linear);
	StringHelp::Float(_file, "QUADRATIC", m_quadratic);
}

void PointLight::SetRenderValues(unsigned int _prog)
{
	Light::SetRenderValues(_prog);

	GLint loc;

	if (Helper::SetUniformLocation(_prog, (m_name + "Constant").c_str(), &loc))
		glUniform1f(loc, m_constant);

	if (Helper::SetUniformLocation(_prog, (m_name + "Linear").c_str(), &loc))
		glUniform1f(loc, m_linear);

	if (Helper::SetUniformLocation(_prog, (m_name + "Quadratic").c_str(), &loc))
		glUniform1f(loc, m_quadratic);
}
