#include "RenderManager.h"
#include "Texture2D.h"
#include "SpriteNoEffectShader.h"

SpriteNoEffectShader::SpriteNoEffectShader(ID3D11Device* device, const std::wstring& vertexShaderSourcePath, const std::wstring& pixelShaderSourcePath)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutElements = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CHECK_HR(CreateVertexShaderFromFile(device, vertexShaderSourcePath, &vertexShaderSource_, &vertexShader_), "failed to create vertex shader...");
	CHECK_HR(CreatePixelShaderFromFile(device, pixelShaderSourcePath, &pixelShaderSource_, &pixelShader_), "failed to create pixel shader...");
	CHECK_HR(CreateInputLayout(device, vertexShaderSource_, inputLayoutElements, &inputLayout_), "failed to create input layout...");

	CHECK_HR(CreateDynamicConstantBuffer<EveryFrameConstantBuffer>(device, &everyFrameBuffer_), "failed to create constant buffer...");
	everyFrameBufferBindSlot_ = 0;

	CHECK_HR(CreateDynamicConstantBuffer<TransparentConstantBuffer>(device, &transparentBuffer_), "failed to create constant buffer...");
	transparentBufferBindSlot_ = 0;

	CHECK_HR(CreateLinearTextureSampler(device, &linearSamplerState_), "failed to create texture sampler...");
	samplerStateBindSlot_ = 0;

	spriteVertex_ = std::vector<Vertex::PositionUV>(4);
	spriteIndex_ = std::vector<uint32_t>{ 0, 1, 2, 0, 2, 3 };

	CHECK_HR(CreateDynamicVertexBuffer<Vertex::PositionUV>(device, spriteVertex_, &spriteVertexBuffer_), "failed to create dynamic vertex buffer...");
	CHECK_HR(CreateIndexBuffer(device, spriteIndex_, &spriteIndexBuffer_), "failed to create index buffer...");
}

SpriteNoEffectShader::~SpriteNoEffectShader()
{
	SAFE_RELEASE(spriteIndexBuffer_);
	SAFE_RELEASE(spriteVertexBuffer_)
	SAFE_RELEASE(linearSamplerState_);
	SAFE_RELEASE(transparentBuffer_);
	SAFE_RELEASE(everyFrameBuffer_);
}

void SpriteNoEffectShader::DrawSprite2D(ID3D11DeviceContext* context, Texture2D* texture, const DirectX::XMFLOAT2& center, float width, float height, float alpha)
{
	EffectShader::Bind(context);

	D3D11_MAPPED_SUBRESOURCE constantBufferMappedResource;

	if (SUCCEEDED(context->Map(everyFrameBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantBufferMappedResource)))
	{
		EveryFrameConstantBuffer* constantBuffer = reinterpret_cast<EveryFrameConstantBuffer*>(constantBufferMappedResource.pData);

		constantBuffer->projection = everyFrameBufferResource_.projection;

		context->Unmap(everyFrameBuffer_, 0);
	}

	if (SUCCEEDED(context->Map(transparentBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantBufferMappedResource)))
	{
		TransparentConstantBuffer* constantBuffer = reinterpret_cast<TransparentConstantBuffer*>(constantBufferMappedResource.pData);

		transparentConstantBuffer.transparent = DirectX::XMFLOAT4(alpha, alpha, alpha, alpha);
		constantBuffer->transparent = transparentConstantBuffer.transparent;

		context->Unmap(transparentBuffer_, 0);
	}

	context->VSSetConstantBuffers(everyFrameBufferBindSlot_, 1, &everyFrameBuffer_);

	textureResource_ = texture->GetTextureView();
	context->PSSetShaderResources(textureResourcebindSlot_, 1, &textureResource_);
	context->PSSetConstantBuffers(transparentBufferBindSlot_, 1, &transparentBuffer_);
	context->PSSetSamplers(samplerStateBindSlot_, 1, &linearSamplerState_);

	UpdateVertexBuffer(context, center, width, height);

	uint32_t stride = sizeof(Vertex::PositionUV);
	uint32_t offset = 0;

	context->IASetVertexBuffers(0, 1, &spriteVertexBuffer_, &stride, &offset);
	context->IASetIndexBuffer(spriteIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->DrawIndexed(static_cast<uint32_t>(spriteIndex_.size()), 0, 0);
}

void SpriteNoEffectShader::UpdateVertexBuffer(ID3D11DeviceContext* context, const DirectX::XMFLOAT2& center, float width, float height)
{
	spriteVertex_[0] = Vertex::PositionUV(
		DirectX::XMFLOAT3(center.x - width / 2.0f, center.y - height / 2.0f, 0.0f),
		DirectX::XMFLOAT2(0.0f, 1.0f)
	);

	spriteVertex_[1] = Vertex::PositionUV(
		DirectX::XMFLOAT3(center.x - width / 2.0f, center.y + height / 2.0f, 0.0f),
		DirectX::XMFLOAT2(0.0f, 0.0f)
	);

	spriteVertex_[2] = Vertex::PositionUV(
		DirectX::XMFLOAT3(center.x + width / 2.0f, center.y + height / 2.0f, 0.0f),
		DirectX::XMFLOAT2(1.0f, 0.0f)
	);

	spriteVertex_[3] = Vertex::PositionUV(
		DirectX::XMFLOAT3(center.x + width / 2.0f, center.y - height / 2.0f, 0.0f),
		DirectX::XMFLOAT2(1.0f, 1.0f)
	);

	D3D11_MAPPED_SUBRESOURCE vertexBufferMappedResource;
	if (SUCCEEDED(context->Map(spriteVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertexBufferMappedResource)))
	{
		Vertex::PositionUV* vertexBuffer = reinterpret_cast<Vertex::PositionUV*>(vertexBufferMappedResource.pData);

		std::memcpy(
			vertexBuffer,
			reinterpret_cast<const void*>(&spriteVertex_[0]),
			spriteVertex_.size() * sizeof(Vertex::PositionUV)
		);

		context->Unmap(spriteVertexBuffer_, 0);
	}
}
