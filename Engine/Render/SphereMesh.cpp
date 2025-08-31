#include "SphereMesh.h"
#include "Resource/ModelLoader.h"
#include <algorithm>
#include <float.h>

SphereMesh::SphereMesh()
{
	// ¸ðµ¨ ·Îµå.
	std::vector<std::weak_ptr<MeshData>> meshList;
	if (ModelLoader::Get().Load("sphere.fbx", meshList))
	{
		for (auto const& mesh : meshList)
		{
			meshes.emplace_back(mesh);
		}
		//meshes.emplace_back(meshList);
	}

}