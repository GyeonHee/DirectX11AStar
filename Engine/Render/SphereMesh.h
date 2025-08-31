#pragma once
#include "Mesh.h"

// »ï°¢Çü ¸Þ½Ã Å¬·¡½º.
class Engine_API SphereMesh : public Mesh
{
	RTTI_DECLARATIONS(SphereMesh, Mesh)
public:
	SphereMesh();
	~SphereMesh() = default;
};