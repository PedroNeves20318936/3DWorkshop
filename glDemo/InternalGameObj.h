#pragma once

#include "core.h"
#include "GameObject.h"
#include "Model.h"

class InternalGameObj : public Model {

protected:
    GLuint m_numFaces = 0;
    GLuint m_vao = 0;
    GLuint m_vertexBuffer = 0;
    GLuint m_colourBuffer = 0;
    GLuint m_indexBuffer = 0;

public:
    InternalGameObj(const float* positionArray, int posSize,
        const float* colourArray, int colSize,
        const unsigned int* indexArray, int indexSize,
        int numFaces);

    virtual ~InternalGameObj();
    virtual void Render() override;
};
