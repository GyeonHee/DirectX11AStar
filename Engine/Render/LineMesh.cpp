#include "LineMesh.h"

LineMesh::LineMesh(const Vector3& start, const Vector3& end)
{
	// ������ ���� �簢������ ���� �ﰢ������ ������
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;

	// ������ ���� ���� ���
	Vector3 direction = end - start;
	direction = direction.Normalized();

	// ���ο� ������ ���� ��� (���� ����)
	Vector3 perpendicular;
	if (abs(direction.y) < 0.9f) {
		perpendicular = Cross(Vector3(0.0f, 1.0f, 0.0f), direction).Normalized();
	}
	else {
		perpendicular = Cross(Vector3(1.0f, 0.0f, 0.0f), direction).Normalized();
	}

	float lineWidth = 0.05f; // ������ �β�
	Vector3 offset = perpendicular * lineWidth * 0.5f;

	// �簢���� �� �𼭸� ���� ����
	Vertex v1, v2, v3, v4;

	v1.position = start - offset;
	v1.normal = Vector3(0.0f, 0.0f, 1.0f);
	v1.color = Vector3(1.0f, 1.0f, 1.0f);
	v1.texCoord = Vector2(0.0f, 0.0f);

	v2.position = start + offset;
	v2.normal = Vector3(0.0f, 0.0f, 1.0f);
	v2.color = Vector3(1.0f, 1.0f, 1.0f);
	v2.texCoord = Vector2(0.0f, 1.0f);

	v3.position = end + offset;
	v3.normal = Vector3(0.0f, 0.0f, 1.0f);
	v3.color = Vector3(1.0f, 1.0f, 1.0f);
	v3.texCoord = Vector2(1.0f, 1.0f);

	v4.position = end - offset;
	v4.normal = Vector3(0.0f, 0.0f, 1.0f);
	v4.color = Vector3(1.0f, 1.0f, 1.0f);
	v4.texCoord = Vector2(1.0f, 0.0f);

	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v4);

	// �簢���� �� ���� �ﰢ������ �����ϴ� �ε���
	// ù ��° �ﰢ��
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	// �� ��° �ﰢ��
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	// MeshData ���� �� �߰�
	auto meshData = std::make_shared<MeshData>(vertices, indices);
	meshes.push_back(meshData);
}