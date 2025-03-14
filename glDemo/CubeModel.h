#pragma once
#include "Model.h"
class Cube;

class CubeModel : public Model
{
public:
    CubeModel();
    virtual ~CubeModel();

    void Load(std::ifstream& _file) override;
    virtual void Render() override;

private:
    Cube* cube;
};
