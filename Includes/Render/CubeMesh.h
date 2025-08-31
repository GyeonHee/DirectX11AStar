#pragma once
#include "Mesh.h"

class Engine_API CubeMesh : public Mesh
{
	RTTI_DECLARATIONS(CubeMesh, Mesh)
public:
	CubeMesh();
	~CubeMesh() = default;
};