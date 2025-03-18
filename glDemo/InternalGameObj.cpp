#include "InternalGameObj.h"

InternalGameObj::InternalGameObj(const float* positionArray, int posSize,
    const float* colourArray, int colSize,
    const unsigned int* indexArray, int indexSize,
    int numFaces)
    : Model(), m_numFaces(numFaces)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // setup vbo for position attribute
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, posSize, positionArray, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
    glEnableVertexAttribArray(0);

    // setup vbo for colour attribute
    glGenBuffers(1, &m_colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, colSize, colourArray, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
    glEnableVertexAttribArray(4);

    // setup vbo for cube) index buffer
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indexArray, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

InternalGameObj::~InternalGameObj() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_colourBuffer);
    glDeleteBuffers(1, &m_indexBuffer);
}

void InternalGameObj::Render() {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_numFaces * 3, GL_UNSIGNED_INT, (const GLvoid*)0);
    glBindVertexArray(0);
}
