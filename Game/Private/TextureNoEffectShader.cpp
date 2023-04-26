#include "Texture2D.h"
#include "TextureNoEffectShader.h"

TextureNoEffectShader::TextureNoEffectShader(ID3D11Device* device, const std::wstring& vertexShaderSourcePath, const std::wstring& pixelShaderSourcePath)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutElements = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CHECK_HR(CreateVertexShaderFromFile(device, vertexShaderSourcePath, &vertexShaderSource_, &vertexShader_), "failed to create vertex shader...");
	CHECK_HR(CreatePixelShaderFromFile(device, pixelShaderSourcePath, &pixelShaderSource_, &pixelShader_), "failed to create pixel shader...");
	CHECK_HR(CreateInputLayout(device, vertexShaderSource_, inputLayoutElements, &inputLayout_), "failed to create input layout...");

	D3D11_BUFFER_DESC everyFrameBufferDesc = {};

	everyFrameBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	everyFrameBufferDesc.ByteWidth = sizeof(EveryFrameConstantBuffer);
	everyFrameBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	everyFrameBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	everyFrameBufferDesc.MiscFlags = 0;
	everyFrameBufferDesc.StructureByteStride = 0;

	CHECK_HR(device->CreateBuffer(&everyFrameBufferDesc, nullptr, &everyFrameBuffer_), "failed to create constant buffer...");

	everyFrameBufferBindSlot_ = 0;

	D3D11_SAMPLER_DESC linearSamplerDesc;
	linearSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	linearSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	linearSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	linearSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	linearSamplerDesc.MipLODBias = 0.0f;
	linearSamplerDesc.MaxAnisotropy = 1;
	linearSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	linearSamplerDesc.BorderColor[0] = 0;
	linearSamplerDesc.BorderColor[1] = 0;
	linearSamplerDesc.BorderColor[2] = 0;
	linearSamplerDesc.BorderColor[3] = 0;
	linearSamplerDesc.MinLOD = 0;
	linearSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	CHECK_HR(device->CreateSamplerState(&linearSamplerDesc, &linearSamplerState_), "failed to create texture sampler...");

	samplerStateBindSlot_ = 0;
}

TextureNoEffectShader::~TextureNoEffectShader()
{
	SAFE_RELEASE(linearSamplerState_);
	SAFE_RELEASE(everyFrameBuffer_);
}

void TextureNoEffectShader::SetTexture(Texture2D* texture)
{
	textureResource_ = texture->GetTextureView();
	textureResourcebindSlot_ = 0;
}

void TextureNoEffectShader::Bind(ID3D11DeviceContext* context)
{
	EffectShader::Bind(context);

	D3D11_MAPPED_SUBRESOURCE everyFrameBufferMappedResource;
	if (SUCCEEDED(context->Map(everyFrameBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &everyFrameBufferMappedResource)))
	{
		EveryFrameConstantBuffer* bufferPtr = reinterpret_cast<EveryFrameConstantBuffer*>(everyFrameBufferMappedResource.pData);
		bufferPtr->world = everyFrameBufferResource_.world;
		bufferPtr->view = everyFrameBufferResource_.view;
		bufferPtr->projection = everyFrameBufferResource_.projection;

		context->Unmap(everyFrameBuffer_, 0);
	}

	context->VSSetConstantBuffers(everyFrameBufferBindSlot_, 1, &everyFrameBuffer_);

	context->PSSetShaderResources(textureResourcebindSlot_, 1, &textureResource_);
	context->PSSetSamplers(samplerStateBindSlot_, 1, &linearSamplerState_);
}