#include "ExampleInternal.h"
#include "AIModel.h"
#include "stringHelp.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"

ExampleInternal::ExampleInternal()
{
}

ExampleInternal::~ExampleInternal()
{
}

void ExampleInternal::Load(ifstream& _file)
{
	GameObject::Load(_file);
	StringHelp::String(_file, "MODEL", m_ModelName);
	StringHelp::String(_file, "SHADER", m_ShaderName);

}

void ExampleInternal::Tick(float _dt)
{
	GameObject::Tick(_dt);
}

void ExampleInternal::PreRender()
{
	GameObject::PreRender();
}

void ExampleInternal::Render()
{
	m_model->Render();
}

void ExampleInternal::Init(Scene* _scene)
{
	m_ShaderProg = _scene->GetShader(m_ShaderName)->GetProg();
	m_model = _scene->GetModel(m_ModelName);
}
