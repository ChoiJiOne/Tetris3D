#include "Texture2D.h"

// @third party code - BEGIN
#include <stb/stb_image.h>
#include <stb/stb_image_resize.h>
// @third party code - END

std::array<int32_t, 10> Texture2D::resolutions_ = {
	16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192
};

Texture2D::Texture2D(ID3D11Device* device, const std::string& path)
{
	std::vector<uint8_t> buffer;
	int32_t textureFormat = 0;
	int32_t textureWidth = 0;
	int32_t textureHeight = 0;

	CHECK(LoadTextureFromFile(path, buffer, textureFormat, textureWidth, textureHeight), "failed to load texture file...");
	CHECK_HR(CreateTextureResource(device, buffer, textureFormat, textureWidth, textureHeight), "failed to create texture resource...");
}

Texture2D::~Texture2D()
{
	SAFE_RELEASE(texture_);
	SAFE_RELEASE(textureView_);
}

bool Texture2D::LoadTextureFromFile(const std::string& path, std::vector<uint8_t>& outBuffer, int32_t& outFormat, int32_t& outWidth, int32_t& outHeight)
{
	uint8_t* resourceBuffer = stbi_load(path.c_str(), &outWidth, &outHeight, &outFormat, STBI_rgb_alpha);

	if (!resourceBuffer) return false;

	std::size_t bufferSize = outWidth * outHeight * outFormat;
	outBuffer.resize(bufferSize);
	std::copy(resourceBuffer, resourceBuffer + bufferSize, &outBuffer[0]);

	stbi_image_free(resourceBuffer);
	resourceBuffer = nullptr;

	bool bIsResizeBuffer = true;
	if (outWidth == outHeight)
	{
		for (std::size_t Index = 0; Index < resolutions_.size(); ++Index)
		{
			if (resolutions_[Index] == outWidth)
			{
				bIsResizeBuffer = false;
			}
		}
	}

	bool bIsSuccessed = true;
	if (bIsResizeBuffer)
	{
		std::vector<uint8_t> newResizeBuffer;
		int32_t newWidth = 0, newHeight = 0;

		bIsSuccessed = ResizeTextureBuffer(outBuffer, outWidth, outHeight, newResizeBuffer, newWidth, newHeight);

		outBuffer = newResizeBuffer;
		outWidth = newWidth;
		outHeight = newHeight;
	}

	return bIsSuccessed;
}

HRESULT Texture2D::CreateTextureResource(ID3D11Device* device, std::vector<uint8_t>& buffer, int32_t format, int32_t width, int32_t height)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Height = static_cast<uint32_t>(width);
	textureDesc.Width = static_cast<uint32_t>(height);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA textueBufferData;
	textueBufferData.pSysMem = reinterpret_cast<const void*>(&buffer[0]);
	textueBufferData.SysMemPitch = width * format;
	textueBufferData.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateTexture2D(&textureDesc, &textueBufferData, &texture_);
	if (SUCCEEDED(hr))
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

		hr = device->CreateShaderResourceView(
			texture_,
			&shaderResourceViewDesc,
			&textureView_
		);
	}

	return hr;
}

Texture2D::EResolution Texture2D::GetResolutionFromSize(int32_t width, int32_t height)
{
	int32_t textureSize = (width > height) ? width : height;
	EResolution textureResolution = EResolution::SIZE_8192X8192;

	for (std::size_t index = 0; index < resolutions_.size() - 1; ++index)
	{
		if (resolutions_[index] <= textureSize && textureSize <= resolutions_[index + 1])
		{
			int32_t diff0 = textureSize - resolutions_[index];
			int32_t diff1 = resolutions_[index + 1] - textureSize;

			textureResolution = (diff0 < diff1) ? static_cast<EResolution>(resolutions_[index]) : static_cast<EResolution>(resolutions_[index + 1]);
			break;
		}
	}

	return textureResolution;
}

bool Texture2D::ResizeTextureBuffer(
	std::vector<uint8_t>& oldBuffer, int32_t oldWidth, int32_t oldHeight,
	std::vector<uint8_t>& outNewBuffer, int32_t& outNewWidth, int32_t& outNewHeight
)
{
	EResolution textureResolution = GetResolutionFromSize(oldWidth, oldHeight);

	outNewWidth = static_cast<int32_t>(textureResolution);
	outNewHeight = static_cast<int32_t>(textureResolution);

	outNewBuffer.resize(outNewWidth * outNewHeight * STBI_rgb_alpha);

	int32_t success = stbir_resize_uint8(
		&oldBuffer[0], oldWidth, oldHeight, 0,
		&outNewBuffer[0], outNewWidth, outNewHeight, 0,
		STBI_rgb_alpha
	);

	return (success != 0);
}