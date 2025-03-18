#pragma once

#include "InternalGameObj.h"

class CGPrincipleAxes : public InternalGameObj {
public:

	CGPrincipleAxes();
	~CGPrincipleAxes();

	virtual void Render() override;

	void render(bool _showZAxis = true);
};
