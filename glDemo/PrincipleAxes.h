#pragma once

#include "core.h"
#include "GameObject.h"
#include "Model.h"

class CGPrincipleAxes : public GameObject, public Model {

private:

	GLuint				m_numFaces = 0;
	GLuint				m_vao = 0;

	GLuint				m_vertexBuffer;
	GLuint				m_colourBuffer;
	GLuint				m_indexBuffer;

public:

	CGPrincipleAxes();
	~CGPrincipleAxes();

	virtual void Render() override;

	void render(bool _showZAxis = true);
};
