#include "GeometryGenerator.h"

inline void GeometryGenerator::CreateBox(float width, float height, float depth, std::vector<Vertex::PositionUV>& outVertices, std::vector<uint32_t>& outIndices)
{
	outVertices.resize(0);
	outIndices.resize(0);

	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	float halfDepth = depth / 2.0f;

	outVertices = {
		Vertex::PositionUV(DirectX::XMFLOAT3(-halfWidth, +halfHeight, -halfDepth), DirectX::XMFLOAT2(1.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+halfWidth, +halfHeight, -halfDepth), DirectX::XMFLOAT2(0.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+halfWidth, +halfHeight, +halfDepth), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-halfWidth, +halfHeight, +halfDepth), DirectX::XMFLOAT2(1.0f, 1.0f)),

		Vertex::PositionUV(DirectX::XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), DirectX::XMFLOAT2(0.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+halfWidth, -halfHeight, -halfDepth), DirectX::XMFLOAT2(1.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+halfWidth, -halfHeight, +halfDepth), DirectX::XMFLOAT2(1.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-halfWidth, -halfHeight, +halfDepth), DirectX::XMFLOAT2(0.0f, 1.0f)),

		Vertex::PositionUV(DirectX::XMFLOAT3(-halfWidth, -halfHeight, +halfDepth), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), DirectX::XMFLOAT2(1.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-halfWidth, +halfHeight, -halfDepth), DirectX::XMFLOAT2(1.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-halfWidth, +halfHeight, +halfDepth), DirectX::XMFLOAT2(0.0f, 0.0f)),

		Vertex::PositionUV(DirectX::XMFLOAT3(+halfWidth, -halfHeight, +halfDepth), DirectX::XMFLOAT2(1.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+halfWidth, -halfHeight, -halfDepth), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+halfWidth, +halfHeight, -halfDepth), DirectX::XMFLOAT2(0.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+halfWidth, +halfHeight, +halfDepth), DirectX::XMFLOAT2(1.0f, 0.0f)),

		Vertex::PositionUV(DirectX::XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+halfWidth, -halfHeight, -halfDepth), DirectX::XMFLOAT2(1.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+halfWidth, +halfHeight, -halfDepth), DirectX::XMFLOAT2(1.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-halfWidth, +halfHeight, -halfDepth), DirectX::XMFLOAT2(0.0f, 0.0f)),

		Vertex::PositionUV(DirectX::XMFLOAT3(-halfWidth, -halfHeight, +halfDepth), DirectX::XMFLOAT2(1.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+halfWidth, -halfHeight, +halfDepth), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+halfWidth, +halfHeight, +halfDepth), DirectX::XMFLOAT2(0.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-halfWidth, +halfHeight, +halfDepth), DirectX::XMFLOAT2(1.0f, 0.0f)),
	};

	outIndices = {
		3, 1, 0,
		2, 1, 3,

		6, 4, 5,
		7, 4 ,6,

		11, 9, 8,
		10, 9, 11,

		14, 12, 13,
		15, 12, 14,
		 
		19, 17, 16,
		18, 17, 19,

		22, 20, 21,
		23, 20, 22,
	};
}
