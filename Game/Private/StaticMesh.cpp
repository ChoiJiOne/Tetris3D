#include "StaticMesh.h"

StaticMesh::StaticMesh(ID3D11Device* device, const std::vector<Vertex::PositionColor>& vertices, const std::vector<uint32_t>& indices)
{
	vertexByteSize_ = sizeof(Vertex::PositionColor);
	countOfIndex_ = static_cast<uint32_t>(indices.size());

	CHECK_HR(CreateVertexBuffer(
		device, 
		vertexByteSize_ * static_cast<uint32_t>(vertices.size()), 
		reinterpret_cast<const void*>(&vertices[0]), 
		&vertexBuffer_
	), "failed to create vertex buffer...");

	CHECK_HR(CreateIndexBuffer(
		device, 
		sizeof(uint32_t) * static_cast<uint32_t>(indices.size()), 
		reinterpret_cast<const void*>(&indices[0]), 
		&indexBuffer_
	), "failed to create index buffer...");
}

StaticMesh::StaticMesh(ID3D11Device* device, const std::vector<Vertex::PositionUV>& vertices, const std::vector<uint32_t>& indices)
{
	vertexByteSize_ = sizeof(Vertex::PositionUV);
	countOfIndex_ = static_cast<uint32_t>(indices.size());

	CHECK_HR(CreateVertexBuffer(
		device,
		vertexByteSize_ * static_cast<uint32_t>(vertices.size()),
		reinterpret_cast<const void*>(&vertices[0]),
		&vertexBuffer_
	), "failed to create vertex buffer...");

	CHECK_HR(CreateIndexBuffer(
		device,
		sizeof(uint32_t) * static_cast<uint32_t>(indices.size()),
		reinterpret_cast<const void*>(&indices[0]),
		&indexBuffer_
	), "failed to create index buffer...");
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

HRESULT StaticMesh::CreateVertexBuffer(ID3D11Device* device, uint32_t byteWidth, const void* bufferPtr, ID3D11Buffer** outVertexBuffer)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = byteWidth;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = bufferPtr;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	return device->CreateBuffer(&vertexBufferDesc, &vertexData, outVertexBuffer);
}

HRESULT StaticMesh::CreateIndexBuffer(ID3D11Device* device, uint32_t byteWidth, const void* bufferPtr, ID3D11Buffer** outIndexBuffer)
{
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = byteWidth;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = bufferPtr;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	return device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer_);
}