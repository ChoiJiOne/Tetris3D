#include <d3dcompiler.h>

#include "StringHelper.hpp"
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
	SAFE_RELEASE(inputLayout_);
	SAFE_RELEASE(pixelShader_);
	SAFE_RELEASE(pixelShaderSource_);
	SAFE_RELEASE(vertexShader_);
	SAFE_RELEASE(vertexShaderSource_);
}

void ColorNoEffectShader::Bind(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(inputLayout_);
	context->VSSetShader(vertexShader_, nullptr, 0);
	context->PSSetShader(pixelShader_, nullptr, 0);

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

HRESULT ColorNoEffectShader::CompileShaderFromFile(const std::wstring& sourcePath, const std::string& entryPoint, const std::string& shaderModel, ID3DBlob** blob)
{
	HRESULT hr = S_OK;

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	hr = D3DCompileFromFile(
		sourcePath.c_str(),
		nullptr,
		nullptr,
		entryPoint.c_str(),
		shaderModel.c_str(),
		shaderFlags,
		0,
		blob,
		nullptr
	);

	return hr;
}

HRESULT ColorNoEffectShader::CreateVertexShaderFromFile(ID3D11Device* device, const std::wstring& sourcePath, ID3DBlob** vertexShaderSource, ID3D11VertexShader** vertexShader)
{
	HRESULT hr = CompileShaderFromFile(sourcePath, "main", "vs_5_0", vertexShaderSource);

	if (SUCCEEDED(hr))
	{
		hr = device->CreateVertexShader(
			(*vertexShaderSource)->GetBufferPointer(),
			(*vertexShaderSource)->GetBufferSize(),
			nullptr,
			vertexShader
		);
	}

	return hr;
}

HRESULT ColorNoEffectShader::CreatePixelShaderFromFile(ID3D11Device* device, const std::wstring& sourcePath, ID3DBlob** pixelShaderSource, ID3D11PixelShader** pixelShader)
{
	HRESULT hr = CompileShaderFromFile(sourcePath, "main", "ps_5_0", pixelShaderSource);

	if (SUCCEEDED(hr))
	{
		hr = device->CreatePixelShader(
			(*pixelShaderSource)->GetBufferPointer(),
			(*pixelShaderSource)->GetBufferSize(),
			nullptr,
			pixelShader
		);
	}

	return hr;
}

HRESULT ColorNoEffectShader::CreateInputLayout(ID3D11Device* device, ID3DBlob* vertexShaderSource, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutElements, ID3D11InputLayout** inputLayout)
{
	return device->CreateInputLayout(
		&inputLayoutElements[0],
		static_cast<uint32_t>(inputLayoutElements.size()),
		vertexShaderSource->GetBufferPointer(),
		vertexShaderSource->GetBufferSize(),
		inputLayout
	);
}