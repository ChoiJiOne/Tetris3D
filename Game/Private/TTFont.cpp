#include "FileHelper.hpp"
#include "TTFont.h"

// @third party code - BEGIN
#include <stb/stb_rect_pack.h>
#include <stb/stb_truetype.h>
// @third party code - END

TTFont::TTFont(ID3D11Device* device, const std::string& path, int32_t beginCodePoint, int32_t endCodePoint, float fontSize)
	: beginCodePoint_(beginCodePoint)
	, endCodePoint_(endCodePoint)
{
	std::vector<uint8_t> buffer;
	FileHelper::ReadBufferFromFile(path, buffer);

	stbtt_fontinfo fontInfo;
	CHECK((stbtt_InitFont(
		&fontInfo,
		reinterpret_cast<const unsigned char*>(&buffer[0]),
		stbtt_GetFontOffsetForIndex(reinterpret_cast<const unsigned char*>(&buffer[0]), 0)
	) != 0), "failed to initialize stb_truetype...");

	std::shared_ptr<uint8_t[]> atlasBitmap = GenerateTextureAtlasBitmap(buffer, beginCodePoint_, endCodePoint_, fontSize, glyphs_, atlasSize_);

	CHECK_HR(CreateTextureAtlasFromBitmap(device, atlasBitmap, atlasSize_), "failed to create texture atlas...");
}

TTFont::~TTFont()
{
	SAFE_RELEASE(textureAtlas_);
	SAFE_RELEASE(textureAtlasView_);
}

const Glyph& TTFont::GetGlyph(int32_t codePoint) const
{
	CHECK(HasCodePointInRange(codePoint), "code point is out of range...");
	int32_t index = codePoint - beginCodePoint_;
	return glyphs_[index];
}

bool TTFont::HasCodePointInRange(int32_t CodePoint) const
{
	return (beginCodePoint_ <= CodePoint) && (CodePoint <= endCodePoint_);
}

std::shared_ptr<uint8_t[]> TTFont::GenerateTextureAtlasBitmap(
	const std::vector<uint8_t>& buffer,
	int32_t beginCodePoint,
	int32_t endCodePoint,
	float fontSize,
	std::vector<Glyph>& outGlyphs,
	int32_t& outAtlasSize
)
{
	std::vector<stbtt_packedchar> packedchars(endCodePoint - beginCodePoint + 1);
	outGlyphs.resize(endCodePoint - beginCodePoint + 1);

	int32_t success = 0;
	stbtt_pack_context packContext;
	std::shared_ptr<uint8_t[]> bitmap = nullptr;

	for (int32_t size = 16; size < 8192; size *= 2)
	{
		bitmap = std::make_unique<uint8_t[]>(size * size);
		success = stbtt_PackBegin(&packContext, bitmap.get(), size, size, 0, 1, nullptr);
		stbtt_PackSetOversampling(&packContext, 1, 1);

		success = stbtt_PackFontRange(
			&packContext,
			reinterpret_cast<const unsigned char*>(&buffer[0]),
			0,
			fontSize,
			beginCodePoint,
			static_cast<int>(packedchars.size()),
			&packedchars[0]
		);

		if (success)
		{
			stbtt_PackEnd(&packContext);
			outAtlasSize = size;
			break;
		}
		else
		{
			stbtt_PackEnd(&packContext);
			bitmap.reset();
		}
	}

	for (std::size_t index = 0; index < packedchars.size(); ++index)
	{
		outGlyphs[index].codePoint = static_cast<int32_t>(index + beginCodePoint);

		outGlyphs[index].position0 = DirectX::XMINT2(packedchars[index].x0, packedchars[index].y0);
		outGlyphs[index].position1 = DirectX::XMINT2(packedchars[index].x1, packedchars[index].y1);

		outGlyphs[index].xoffset = packedchars[index].xoff;
		outGlyphs[index].yoffset = packedchars[index].yoff;

		outGlyphs[index].xoffset2 = packedchars[index].xoff2;
		outGlyphs[index].yoffset2 = packedchars[index].yoff2;

		outGlyphs[index].xadvance = packedchars[index].xadvance;
	}

	return bitmap;
}

HRESULT TTFont::CreateTextureAtlasFromBitmap(ID3D11Device* device, const std::shared_ptr<uint8_t[]>& bitmap, const int32_t& atlasSize)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Height = static_cast<uint32_t>(atlasSize);
	textureDesc.Width = static_cast<uint32_t>(atlasSize);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA textueBufferData;
	textueBufferData.pSysMem = reinterpret_cast<const void*>(&bitmap[0]);
	textueBufferData.SysMemPitch = atlasSize;
	textueBufferData.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateTexture2D(&textureDesc, &textueBufferData, &textureAtlas_);
	if (SUCCEEDED(hr))
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		shaderResourceViewDesc.Format = DXGI_FORMAT_R8_UNORM;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

		hr = device->CreateShaderResourceView(
			textureAtlas_,
			&shaderResourceViewDesc,
			&textureAtlasView_
		);
	}

	return hr;
}