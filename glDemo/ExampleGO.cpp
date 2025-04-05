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

    // this snippet of the code here servers the purpose of adding my desired possitions into the "m_modelOffsets" so that they can be re-rendered
    auto Add = [&](float x, float y, float z) {
        m_modelOffsets.push_back(glm::vec3(x, y, z));
        };

    // this "while" will go through all words of my _file (manifest.txt) 
    while (_file)
    {
        // every word will be then stored momentarelly into a variable that I created called (token)
        std::string token;
        _file >> token;

        // this token would then be verified a couple times, by seeing if its equal to any of my predetermined "Keywords",
        // the first keyword that I created is "OFFSET:", basically if the token sees this word it would then add its three next numbers as
        // x,y,z coordinates so that they can be processed again in the "Render" function, (this was created mainly for tests, as you can see that its
        // not beeing utilized in my manifest for the final version, this is due to it requiring many lines to create large structures so it would clutter the manifest file)
        if (token == "OFFSET:")
        {
            float x, y, z;
            _file >> x >> y >> z;
            Add(x, y, z);
        }
        // the next keyword is in relation to the creation of prefabs (basically the creation of a copy)
        // if my code sees the "PREFAB:" keyword it would then store the next name found in the manifest file (this would be atribuited to the
        // type of prefab that I wish to be created), so for an example if my code saw this line "PREFAB: OUTWARDSROOM" it would store "OUTWARDSROOM"
        // into a variable called "prefabName" and compare it to all of my possible keywords, if it matches any it would then add all of their assigned 
        // coordinates to be re-rendered, therefore creating a large structure at once and only calling one game object in the manifest file 
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

    // if my list of coordinates is found empty "meaning that it doesent have any prefabs or offsets" I just add a set of coordinates on the origin
    // this will make sure that only one object will be instantiated and that it would be right on the origin 
    // "basically just setting them to be on the "POS:" that is set in the manifest file" 
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
    // this loop renders the model multiple times, once per added position on the (m_modelOffsets)
    // this structure allows me to re-utilize the same model multiple times without having to call multiple different gameobjects in the manifest
    for (const auto& offset : m_modelOffsets)
    {
        // this line assigns the exact position for every instance, by getting their world matrix and adding the offsets that we assigned
        // therefore leading each piece to its correct assigned position
        glm::mat4 localMatrix = glm::translate(m_worldMatrix, offset);

        // this section sends the updated model matrix to the shader
        GLint pLocation;
        Helper::SetUniformLocation(m_ShaderProg, "modelMatrix", &pLocation);
        glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&localMatrix);

        // renders the object in the correct position
        m_model->Render();
    }
}

void ExampleGO::Init(Scene* _scene)
{
	m_ShaderProg = _scene->GetShader(m_ShaderName)->GetProg();
	m_texture = _scene->GetTexture(m_TexName)->GetTexID();
	m_model = _scene->GetModel(m_ModelName);
}
