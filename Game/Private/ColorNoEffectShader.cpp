#include "ColorNoEffectShader.h"

ColorNoEffectShader::ColorNoEffectShader(ID3D11Device* device, const std::wstring& vertexShaderSourcePath, const std::wstring& pixelShaderSourcePath)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutElements = {
		{ "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{    "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
}

ColorNoEffectShader::~ColorNoEffectShader()
{
	SAFE_RELEASE(everyFrameBuffer_);
}

void ColorNoEffectShader::Bind(ID3D11DeviceContext* context)
{
	EffectShader::Bind(context);

	D3D11_MAPPED_SUBRESOURCE everyFrameBufferMappedResource;
	if (SUCCEEDED(context->Map(everyFrameBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &everyFrameBufferMappedResource)))
	{
		EveryFrameConstantBuffer* bufferPtr = reinterpret_cast<EveryFrameConstantBuffer*>(everyFrameBufferMappedResource.pData);
		bufferPtr->world = everyFrameBufferResource_.world;
		bufferPtr->view = everyFrameBufferResource_.view;
		bufferPtr->projection= everyFrameBufferResource_.projection;

		context->Unmap(everyFrameBuffer_, 0);
	}

	context->VSSetConstantBuffers(everyFrameBufferBindSlot_, 1, &everyFrameBuffer_);
}