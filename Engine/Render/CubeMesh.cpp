#include "CubeMesh.h"
#include "Resource/ModelLoader.h"
#include <algorithm>
#include <float.h>

CubeMesh::CubeMesh()
{
	std::vector<std::weak_ptr<MeshData>> meshList;
	if (ModelLoader::Get().Load("cube.fbx", meshList))
	{
		for (auto const& mesh : meshList)
		{
			meshes.emplace_back(mesh);
		}
	}
}