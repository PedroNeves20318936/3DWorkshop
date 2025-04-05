#include "ExampleGO.h"
#include "AIModel.h"
#include "stringHelp.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"
#include <helper.h>

ExampleGO::ExampleGO()
{
}

ExampleGO::~ExampleGO()
{
}

void ExampleGO::Load(ifstream& _file)
{
    GameObject::Load(_file);
    StringHelp::String(_file, "MODEL", m_ModelName);
    StringHelp::String(_file, "TEXTURE", m_TexName);
    StringHelp::String(_file, "SHADERS", m_ShaderName);

    auto Add = [&](float x, float y, float z) {
        m_modelOffsets.push_back(glm::vec3(x, y, z));
        };

    while (_file)
    {
        std::string token;
        _file >> token;

        if (token == "OFFSET:")
        {
            float x, y, z;
            _file >> x >> y >> z;
            Add(x, y, z);
        }
        else if (token == "PREFAB:")
        {
            std::string prefabName;
            _file >> prefabName;

            if (prefabName == "FOURWAYROOM")
            {
                // Central Row
                Add(0, 0, 0);
                Add(2, 0, 0);
                Add(-2, 0, 0);
                Add(4, 0, 0);
                Add(-4, 0, 0);
                Add(6, 0, 0);
                Add(-6, 0, 0);
                // Right Row (Long)
                Add(0, 0, 2);
                Add(2, 0, 2);
                Add(-2, 0, 2);
                Add(4, 0, 2);
                Add(-4, 0, 2);
                Add(6, 0, 2);
                Add(-6, 0, 2);
                // Left Row (Long)
                Add(0, 0, -2);
                Add(2, 0, -2);
                Add(-2, 0, -2);
                Add(4, 0, -2);
                Add(-4, 0, -2);
                Add(6, 0, -2);
                Add(-6, 0, -2);
                // Right Row (Short)
                Add(0, 0, 4);
                Add(2, 0, 4);
                Add(-2, 0, 4);
                Add(4, 0, 4);
                Add(-4, 0, 4);
                Add(0, 0, 6);
                Add(-2, 0, 6);
                Add(2, 0, 6);
                // Left Row (Short)
                Add(0, 0, -4);
                Add(2, 0, -4);
                Add(-2, 0, -4);
                Add(4, 0, -4);
                Add(-4, 0, -4);
                Add(0, 0, -6);
                Add(-2, 0, -6);
                Add(2, 0, -6);
                // Forward Wall
                Add(6, 2, 4);
                Add(6, 2, -4);
                // Backwards Wall
                Add(-6, 2, 4);
                Add(-6, 2, -4);
                // Right Wall
                Add(4, 2, 6);
                Add(-4, 2, 6);
                // Left Wall
                Add(4, 2, -6);
                Add(-4, 2, -6);
            }
            else if (prefabName == "CORRIDOR")
            {
                // Floor
                Add(0, 0, 0);
                Add(0, 0, 2);
                Add(0, 0, -2);
                Add(2, 0, 0);
                Add(2, 0, 2);
                Add(2, 0, -2);
                Add(4, 0, 0);
                Add(4, 0, 2);
                Add(4, 0, -2);
                Add(0, 0, -6);
                Add(0, 0, -8);
                Add(0, 0, -10);
                Add(0, 0, -12);
                Add(2, 0, -6);
                Add(2, 0, -8);
                Add(2, 0, -10);
                Add(2, 0, -12);
                Add(4, 0, -6);
                Add(4, 0, -8);
                Add(4, 0, -10);
                Add(4, 0, -12);
                Add(-2, 0, -6);
                Add(-2, 0, -8);
                Add(-2, 0, -10);
                Add(-2, 0, -12);
                // Right Wall
                Add(0, 0, 4);
                Add(2, 0, 4);
                Add(4, 2, 4);
                // Left Wall
                Add(0, 0, -4);
                Add(2, 0, -4);
                Add(4, 2, -4);
            }
            else if (prefabName == "OUTWARDSROOM")
            {
                // Central Row
                Add(0, 0, 0);
                Add(2, 0, 0);
                Add(-2, 0, 0);
                Add(4, 0, 0);
                Add(-4, 0, 0);
                Add(6, 0, 0);
                // Right Row (Long)
                Add(0, 0, 2);
                Add(2, 0, 2);
                Add(-2, 0, 2);
                Add(4, 0, 2);
                Add(-4, 0, 2);
                Add(6, 0, 2);
                // Left Row (Long)
                Add(0, 0, -2);
                Add(2, 0, -2);
                Add(-2, 0, -2);
                Add(4, 0, -2);
                Add(-4, 0, -2);
                Add(6, 0, -2);
                // Right Row (Short)
                Add(0, 0, 4);
                Add(2, 0, 4);
                Add(-2, 0, 4);
                Add(4, 0, 4);
                Add(-4, 0, 4);
                Add(0, 0, 6);
                Add(-2, 0, 6);
                Add(2, 0, 6);
                // Left Row (Short)
                Add(0, 0, -4);
                Add(2, 0, -4);
                Add(-2, 0, -4);
                Add(4, 0, -4);
                Add(-4, 0, -4);
                Add(0, 0, -6);
                Add(-2, 0, -6);
                Add(2, 0, -6);
                // Forward Wall
                Add(6, 2, 4);
                Add(6, 2, -4);
                Add(8, 2, -2);
                Add(8, 2, 0);
                Add(8, 2, 2);
                // Right Wall
                Add(4, 2, 6);
                Add(-4, 2, 6);
                // Left Wall
                Add(4, 2, -6);
                Add(-4, 2, -6);
                // Corridor
                Add(0, 0, 8);
                Add(2, 0, 8);
                Add(-2, 0, 8);
                Add(0, 0, 10);
                Add(2, 0, 10);
                Add(-2, 0, 10);
                Add(0, 0, 12);
                Add(2, 0, 12);
                Add(-2, 0, 12);
                Add(0, 0, 14);
                Add(2, 0, 14);
                Add(-2, 0, 14);
                Add(0, 0, 16);
                Add(2, 0, 16);
                Add(-2, 0, 16);
                Add(0, 0, 18);
                Add(2, 0, 18);
                Add(-2, 0, 18);
                Add(0, 0, 20);
                Add(2, 0, 20);
                Add(-2, 0, 20);
                Add(-4, 0, 18);
                Add(-4, 0, 16);
                Add(-4, 0, 20);
                Add(-6, 0, 18);
                Add(-6, 0, 16);
                Add(-6, 0, 20);
                Add(-8, 0, 18);
                Add(-8, 0, 16);
                Add(-8, 0, 20);
                Add(-10, 0, 18);
                Add(-10, 0, 16);
                Add(-10, 0, 20);
                // Corridor Wall
                Add(4, 2, 8);
                Add(-4, 2, 8);
                Add(4, 2, 10);
                Add(-4, 2, 10);
                Add(4, 2, 12);
                Add(-4, 2, 12);
                Add(4, 2, 14);
                Add(-4, 2, 14);
                Add(4, 2, 16);
                Add(4, 2, 18);
                Add(4, 2, 20);
                Add(4, 2, 22);
                Add(2, 2, 22);
                Add(0, 2, 22);
                Add(-2, 2, 22);
                Add(-4, 2, 22);
                Add(-6, 2, 22);
                Add(-6, 2, 14);
                Add(-8, 2, 22);
                Add(-8, 2, 14);
                Add(-10, 2, 22);
                Add(-10, 2, 14);
            }
        }
        else if (token == "}")
        {
            break;
        }
    }

    if (m_modelOffsets.empty())
        Add(0, 0, 0);
}

void ExampleGO::Tick(float _dt)
{
	GameObject::Tick(_dt);
}

void ExampleGO::PreRender()
{
	GameObject::PreRender();

	//only thing I need to do is tell the shader about my texture

	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	//TODO: this does sort of replicate stuff in the AIMesh class, could we make them more compatible.

	//TODO: NORMAL MAPS!
}

void ExampleGO::Render()
{
    for (const auto& offset : m_modelOffsets)
    {
        glm::mat4 localMatrix = glm::translate(m_worldMatrix, offset);

        GLint pLocation;
        Helper::SetUniformLocation(m_ShaderProg, "modelMatrix", &pLocation);
        glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&localMatrix);

        m_model->Render();
    }
}

void ExampleGO::Init(Scene* _scene)
{
	m_ShaderProg = _scene->GetShader(m_ShaderName)->GetProg();
	m_texture = _scene->GetTexture(m_TexName)->GetTexID();
	m_model = _scene->GetModel(m_ModelName);
}
