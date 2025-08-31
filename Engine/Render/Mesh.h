#pragma once

#include "Core.h"
#include "Math/Transform.h"
#include "Vertex.h"
#include "RTTI.h"
#include <vector>
#include <memory>

struct MeshData
{
	MeshData();
	MeshData(const std::vector<Vertex>& vertices,
		const std::vector<uint32>& indices);
	~MeshData();

	void Bind();
	uint32 IndexCount() const { return (uint32)indices.size(); }

	// ���� �����͸� ������Ʈ�ϴ� �Լ�.
	void UpdateVertexBuffer(const std::vector<Vertex>& vertices);

	// ���� ������.
	std::vector<Vertex> vertices;
	uint32 stride = 0;
	ID3D11Buffer* vertexBuffer = nullptr;

	// �ε��� ������.
	std::vector<uint32> indices;
	ID3D11Buffer* indexBuffer = nullptr;
};

class Engine_API Mesh : public RTTI
{
	RTTI_DECLARATIONS(Mesh, RTTI)
public:
	Mesh();
	virtual ~Mesh() = default;

	// ���� �޽� ���� ��ȯ Getter.
	uint32 SubMeshCount() const;

	// ���� �޽� ��ȯ Getter.
	std::weak_ptr<MeshData> GetSubMesh(int index) const;

protected:
	std::vector<std::shared_ptr<MeshData>> meshes;

};