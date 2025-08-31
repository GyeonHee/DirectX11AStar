#include "LineMesh.h"

LineMesh::LineMesh(const Vector3& start, const Vector3& end)
{
	// 라인을 얇은 사각형으로 만들어서 삼각형으로 렌더링
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;

	// 라인의 방향 벡터 계산
	Vector3 direction = end - start;
	direction = direction.Normalized();

	// 라인에 수직인 벡터 계산 (폭을 위해)
	Vector3 perpendicular;
	if (abs(direction.y) < 0.9f) {
		perpendicular = Cross(Vector3(0.0f, 1.0f, 0.0f), direction).Normalized();
	}
	else {
		perpendicular = Cross(Vector3(1.0f, 0.0f, 0.0f), direction).Normalized();
	}

	float lineWidth = 0.05f; // 라인의 두께
	Vector3 offset = perpendicular * lineWidth * 0.5f;

	// 사각형의 네 모서리 정점 생성
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

	// 사각형을 두 개의 삼각형으로 분할하는 인덱스
	// 첫 번째 삼각형
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	// 두 번째 삼각형
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	// MeshData 생성 및 추가
	auto meshData = std::make_shared<MeshData>(vertices, indices);
	meshes.push_back(meshData);
}