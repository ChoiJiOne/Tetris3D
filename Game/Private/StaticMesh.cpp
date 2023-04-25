#include "StaticMesh.h"

StaticMesh::StaticMesh(ID3D11Device* device, const std::vector<Vertex::PositionColor>& vertices, const std::vector<uint32_t>& indices)
{
	vertexByteSize_ = sizeof(Vertex::PositionColor);
	countOfIndex_ = static_cast<uint32_t>(indices.size());

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = vertexByteSize_ * static_cast<uint32_t>(vertices.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = reinterpret_cast<const void*>(&vertices[0]);
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	CHECK_HR(device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer_), "failed to create vertex buffer...");

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(uint32_t) * static_cast<uint32_t>(indices.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = reinterpret_cast<const void*>(&indices[0]);
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	CHECK_HR(device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer_), "failed to create index buffer...");
}

StaticMesh::~StaticMesh()
{
	SAFE_RELEASE(indexBuffer_);
	SAFE_RELEASE(vertexBuffer_);
}

void StaticMesh::Draw(ID3D11DeviceContext* context)
{
	uint32_t stride = vertexByteSize_;
	uint32_t offset = 0;

	context->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->DrawIndexed(countOfIndex_, 0, 0);
}