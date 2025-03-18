#include "PrincipleAxes.h"


using namespace std;
using namespace glm;


// Example data for principle axes

// Packed vertex buffer for principle axes model
static float positionArray[] = {

	// x axis model
	0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.025f, 0.025f, 0.0, 1.0f,
	1.075f, 0.075f, 0.0, 1.0f,
	1.025f, 0.075f, 0.0, 1.0f,
	1.075f, 0.025f, 0.0, 1.0f,

	// y axis model
	0.0, 0.0, 0.0, 1.0f,
	0.0, 1.0, 0.0, 1.0f,
	-0.075f, 1.075f, 0.0, 1.0f,
	-0.05f, 1.05f, 0.0, 1.0f,
	-0.025f, 1.075f, 0.0, 1.0f,
	-0.075f, 1.025f, 0.0, 1.0f,

	// z axis model
	0.0, 0.0, 0.0, 1.0f,
	0.0, 0.0, 1.0, 1.0f,
	0.025f, 0.075f, 1.0, 1.0f,
	0.075f, 0.075f, 1.0, 1.0f,
	0.025f, 0.025f, 1.0, 1.0f,
	0.075f, 0.025f, 1.0, 1.0f
};

// Packed colour buffer for principle axes model
static float colourArray[] = {

	// x axis colour (red)
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

	// y axis colour (green)
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,

	// z axis colour (blue)
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f
};


// Line list topology to render principle axes
static unsigned int indexArray[] = {

	0, 1, 2, 3, 4, 5,					// x axis
	6, 7, 8, 9, 10, 11,					// y axis
	12, 13, 14, 15, 15, 16, 16, 17		// z axis
};



CGPrincipleAxes::CGPrincipleAxes() 
	: InternalGameObj(positionArray, sizeof(positionArray),
		colourArray, sizeof(colourArray),
		indexArray, sizeof(indexArray),
		10) {
}


CGPrincipleAxes::~CGPrincipleAxes() {}

void CGPrincipleAxes::Render() 
{
	render(true);
}


void CGPrincipleAxes::render(bool _showZAxis) {
	glBindVertexArray(m_vao);
	glDrawElements(GL_LINES, m_numFaces * 3, GL_UNSIGNED_INT, (const GLvoid*)0);
}
