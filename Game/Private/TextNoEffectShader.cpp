#include "RenderManager.h"
#include "TTFont.h"
#include "TextNoEffectShader.h"

TextNoEffectShader::TextNoEffectShader(ID3D11Device* device, const std::wstring& vertexShaderSourcePath, const std::wstring& pixelShaderSourcePath)
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

	CHECK_HR(CreateDynamicConstantBuffer<TextColorConstantBuffer>(device, &textColorBuffer_), "failed to create constant buffer...");
	textColorBufferBindSlot_ = 0;

	CHECK_HR(CreateLinearTextureSampler(device, &linearSamplerState_), "failed to create texture sampler...");
	samplerStateBindSlot_ = 0;

	characterVertex_ = std::vector<Vertex::PositionUV>(4);
	characterIndex_ = std::vector<uint32_t>{ 0, 1, 2, 0, 2, 3 };

	CHECK_HR(CreateDynamicVertexBuffer<Vertex::PositionUV>(device, characterVertex_, &characterVertexBuffer_), "failed to create dynamic vertex buffer...");
	CHECK_HR(CreateIndexBuffer(device, characterIndex_, &characterIndexBuffer_), "failed to create index buffer...");
}

TextNoEffectShader::~TextNoEffectShader()
{
	SAFE_RELEASE(characterIndexBuffer_);
	SAFE_RELEASE(characterVertexBuffer_);
	SAFE_RELEASE(linearSamplerState_);
	SAFE_RELEASE(textColorBuffer_);
	SAFE_RELEASE(everyFrameBuffer_);
}

void TextNoEffectShader::DrawText2D(ID3D11DeviceContext* context, TTFont* font, const std::wstring& text, const DirectX::XMFLOAT2& center, const DirectX::XMFLOAT4& color)
{
	float textWidth = 0.0f, textHeight = 0.0f;
	font->MeasureText<float>(text, textWidth, textHeight);

	float atlasSize = static_cast<float>(font->GetAtlasSize());
	DirectX::XMFLOAT2 position(center.x - textWidth / 2.0f, center.y - textHeight / 2.0f);
	
	for (const auto& unicode : text)
	{
		EffectShader::Bind(context);

		const Glyph& glyph = font->GetGlyph(static_cast<int32_t>(unicode));
		UpdateVertexBuffer(context, glyph, atlasSize, position);

		uint32_t stride = sizeof(Vertex::PositionUV);
		uint32_t offset = 0;

		context->IASetVertexBuffers(0, 1, &characterVertexBuffer_, &stride, &offset);
		context->IASetIndexBuffer(characterIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D11_MAPPED_SUBRESOURCE constantBufferMappedResource;

		if (SUCCEEDED(context->Map(everyFrameBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantBufferMappedResource)))
		{
			EveryFrameConstantBuffer* constantBuffer = reinterpret_cast<EveryFrameConstantBuffer*>(constantBufferMappedResource.pData);
			
			constantBuffer->projection = everyFrameBufferResource_.projection;

			context->Unmap(everyFrameBuffer_, 0);
		}

		if (SUCCEEDED(context->Map(textColorBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantBufferMappedResource)))
		{
			TextColorConstantBuffer* constantBuffer = reinterpret_cast<TextColorConstantBuffer*>(constantBufferMappedResource.pData);

			constantBuffer->textColor = color;

			context->Unmap(textColorBuffer_, 0);
		}

		context->VSSetConstantBuffers(everyFrameBufferBindSlot_, 1, &everyFrameBuffer_);
		
		ID3D11ShaderResourceView* textureAtlasView = font->GetTextureAtlasView();
		context->PSSetShaderResources(textureAtlasResourcebindSlot_, 1, &textureAtlasView);
		context->PSSetConstantBuffers(textColorBufferBindSlot_, 1, &textColorBuffer_);
		context->PSSetSamplers(samplerStateBindSlot_, 1, &linearSamplerState_);

		context->DrawIndexed(static_cast<uint32_t>(characterIndex_.size()), 0, 0);

		position.x += glyph.xadvance;
	}
}

void TextNoEffectShader::UpdateVertexBuffer(ID3D11DeviceContext* context, const Glyph& glyph, float atlasSize, const DirectX::XMFLOAT2& position)
{
	float unicodeWidth = static_cast<float>(glyph.position1.x - glyph.position0.x);
	float unicodeHeight = static_cast<float>(glyph.position1.y - glyph.position0.y);

	characterVertex_[0] = Vertex::PositionUV(
		DirectX::XMFLOAT3(
			position.x + glyph.xoffset, 
			position.y - unicodeHeight - glyph.yoffset, 
			0.0f
		),
		DirectX::XMFLOAT2(
			static_cast<float>(glyph.position0.x) / atlasSize,
			static_cast<float>(glyph.position1.y) / atlasSize
		)
	);

	characterVertex_[1] = Vertex::PositionUV(
		DirectX::XMFLOAT3(
			position.x + glyph.xoffset, 
			position.y - glyph.yoffset, 
			0.0f
		),
		DirectX::XMFLOAT2(
			static_cast<float>(glyph.position0.x) / atlasSize,
			static_cast<float>(glyph.position0.y) / atlasSize
		)
	);

	characterVertex_[2] = Vertex::PositionUV(
		DirectX::XMFLOAT3(
			position.x + glyph.xoffset + unicodeWidth, 
			position.y - glyph.yoffset, 
			0.0f
		),
		DirectX::XMFLOAT2(
			static_cast<float>(glyph.position1.x) / atlasSize,
			static_cast<float>(glyph.position0.y) / atlasSize
		)
	);

	characterVertex_[3] = Vertex::PositionUV(
		DirectX::XMFLOAT3(
			position.x + glyph.xoffset + unicodeWidth, 
			position.y - unicodeHeight - glyph.yoffset, 
			0.0f
		),
		DirectX::XMFLOAT2(
			static_cast<float>(glyph.position1.x) / atlasSize,
			static_cast<float>(glyph.position1.y) / atlasSize
		)
	);

	D3D11_MAPPED_SUBRESOURCE vertexBufferMappedResource;

	if (SUCCEEDED(context->Map(characterVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertexBufferMappedResource)))
	{
		Vertex::PositionUV* vertexBuffer = reinterpret_cast<Vertex::PositionUV*>(vertexBufferMappedResource.pData);

		std::memcpy(
			vertexBuffer,
			reinterpret_cast<const void*>(&characterVertex_[0]),
			characterVertex_.size() * sizeof(Vertex::PositionUV)
		);

		context->Unmap(characterVertexBuffer_, 0);
	}
}
