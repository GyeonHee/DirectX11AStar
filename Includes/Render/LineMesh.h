#pragma once
#include "Mesh.h"

class Engine_API LineMesh : public Mesh
{
	RTTI_DECLARATIONS(LineMesh, Mesh)
public:
	LineMesh(const Vector3& start, const Vector3& end);
	~LineMesh() = default;
};