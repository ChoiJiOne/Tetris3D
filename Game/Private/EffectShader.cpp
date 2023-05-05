#include <d3dcompiler.h>

#include "StringHelper.hpp"
#include "EffectShader.h"

EffectShader::~EffectShader()
{
	SAFE_RELEASE(inputLayout_);
	SAFE_RELEASE(pixelShader_);
	SAFE_RELEASE(pixelShaderSource_);
	SAFE_RELEASE(vertexShader_);
	SAFE_RELEASE(vertexShaderSource_);
}

void EffectShader::Bind(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(inputLayout_);
	context->VSSetShader(vertexShader_, nullptr, 0);
	context->PSSetShader(pixelShader_, nullptr, 0);
}

HRESULT EffectShader::CompileShaderFromFile(const std::wstring& sourcePath, const std::string& entryPoint, const std::string& shaderModel, ID3DBlob** blob)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	return D3DCompileFromFile(
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
}

HRESULT EffectShader::CreateVertexShaderFromFile(ID3D11Device* device, const std::wstring& sourcePath, ID3DBlob** vertexShaderSource, ID3D11VertexShader** vertexShader)
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

HRESULT EffectShader::CreatePixelShaderFromFile(ID3D11Device* device, const std::wstring& sourcePath, ID3DBlob** pixelShaderSource, ID3D11PixelShader** pixelShader)
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

HRESULT EffectShader::CreateInputLayout(ID3D11Device* device, ID3DBlob* vertexShaderSource, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutElements, ID3D11InputLayout** inputLayout)
{
	return device->CreateInputLayout(
		&inputLayoutElements[0],
		static_cast<uint32_t>(inputLayoutElements.size()),
		vertexShaderSource->GetBufferPointer(),
		vertexShaderSource->GetBufferSize(),
		inputLayout
	);
}

HRESULT EffectShader::CreateIndexBuffer(ID3D11Device* device, const std::vector<uint32_t>& indices, ID3D11Buffer** outIndexBuffer)
{
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

	return device->CreateBuffer(&indexBufferDesc, &indexData, outIndexBuffer);
}
