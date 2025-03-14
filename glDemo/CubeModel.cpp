#include "CubeModel.h"
#include "stringHelp.h"
#include "Cube.h"
#include <iostream>

CubeModel::CubeModel()
{
    m_type = "CUBE";
    cube = nullptr;
}

CubeModel::~CubeModel()
{
    delete cube;
}

void CubeModel::Load(std::ifstream& _file)
{
    Model::Load(_file);

    cube = new Cube();
}

void CubeModel::Render()
{
    if (cube)
    {
        cube->render();
    }
}

