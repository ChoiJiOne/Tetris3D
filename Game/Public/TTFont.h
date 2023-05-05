#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <vector>
#include <string>
#include <memory>

#include "Macro.h"


/**
 * @brief 텍스처 아틀라스에 표시된 문자 정보입니다.
 */
struct Glyph
{
	int32_t codePoint;
	DirectX::XMINT2 position0;
	DirectX::XMINT2 position1;
	float xoffset;
	float yoffset;
	float xoffset2;
	float yoffset2;
	float xadvance;
};


/**
 * @brief 트루 타입 폰트에 대응하는 문자들의 텍스처 아틀라스입니다.
 */
class TTFont
{
public:
	/**
	 * @brief 트루 타입 폰트를 로딩하고 텍스트 텍스처 아틀라스를 생성하는 폰트의 생성자입니다.
	 *
	 * @param device 텍스처 아틀라스를 생성하는 데 사용할 디바이스입니다.
	 * @param path 트루 타입 폰트 파일의 경로입니다.
	 * @param beginCodePoint 문자 텍스처 아틀라스의 코드 포인트 시작점입니다.
	 * @param endCodePoint 문자 텍스처 아틀라스의 코드 포인트 끝점입니다.
	 * @param fontSize 폰트의 크기입니다.
	 *
	 * @throws
	 * 트루 타입 폰트 로딩에 실패하면 C++ 표준 예외를 던집니다.
	 * 텍스처 아틀라스 생성에 실패하면 C++ 표준 예외를 던집니다.
	 */
	TTFont(ID3D11Device* device, const std::string& path, int32_t beginCodePoint, int32_t endCodePoint, float fontSize);


	/**
	 * @brief 트루 타입 폰트의 가상 소멸자입니다.
	 */
	virtual ~TTFont();


	/**
	 * @brief 복사 생성자와 대입 연산자를 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(TTFont);


	/**
	 * @brief 텍스처 아틀라스 데이터를 얻습니다.
	 *
	 * @return 2D 텍스처 아틀라스 데이터를 얻습니다.
	 */
	ID3D11Texture2D* GetTextureAtlas() { return textureAtlas_; }


	/**
	 * @brief 셰이더에 바인딩 할 텍스처 아틀라스 리소스 뷰를 얻습니다.
	 *
	 * @return 셰이더에 바인딩 할 텍스처 아틀라스 리소스 뷰를 반환합니다.
	 */
	ID3D11ShaderResourceView* GetTextureAtlasView() { return textureAtlasView_; }


	/**
	 * @brief 코드 포인트에 대응하는 문자 위치 및 크기 정보를 얻습니다.
	 *
	 * @param codePoint 위치 및 정보를 알고 싶은 코드 포인트 값입니다.
	 *
	 * @throws 코드 포인트 값이 범위 내에 없으면 C++ 표준 예외를 던집니다.
	 *
	 * @return 코드 포인트에 대응하는 문자의 위치 및 크기 정보를 반환합니다.
	 */
	const Glyph& GetGlyph(int32_t codePoint) const;


	/**
	 * @brief 텍스처 아틀라스의 크기를 얻습니다.
	 *
	 * @return 텍스처 아틀라스의 크기를 반환합니다.
	 */
	int32_t GetAtlasSize() const { return atlasSize_; }


	/**
	 * @brief 코드 포인트가 범위 내에 포함되는지 확인합니다.
	 *
	 * @param codePoint 범위 검사를 수행할 코드 포인트 값입니다.
	 *
	 * @return 범위 내에 있다면 true, 그렇지 않다면 false를 반환합니다.
	 */
	bool HasCodePointInRange(int32_t codePoint) const;


	/**
	 * @brief 텍스트의 크기를 측정합니다.
	 *
	 * @note 측정 단위는 픽셀 단위입니다.
	 *
	 * @param text 측정할 텍스트입니다.
	 * @param outWidth[out] 측정한 텍스트의 가로 크기입니다.
	 * @param outHeight[out] 측정한 텍스트의 세로 크기입니다.
	 */
	template <typename T>
	void MeasureText(const std::wstring& text, T& outWidth, T& outHeight) const
	{
		int32_t textHeight = -1;
		int32_t textWidth = 0;

		for (const auto& unicode : text)
		{
			const Glyph& glyph = GetGlyph(static_cast<int32_t>(unicode));

			int32_t currentWidth = static_cast<int32_t>(glyph.xadvance);
			int32_t currentHeight = glyph.position1.y - glyph.position0.y;

			textWidth += currentWidth;

			if (currentHeight > textHeight)
			{
				textHeight = currentHeight;
			}
		}

		outWidth = static_cast<T>(textWidth);
		outHeight = static_cast<T>(textHeight);
	}


private:
	/**
	 * @brief 텍스처 아틀라스 비트맵을 생성합니다.
	 *
	 * @note 텍스처 아틀라스의 가로 세로 크기는 같습니다.
	 *
	 * @param buffer 트루 타입 폰트 리소스 버퍼입니다.
	 * @param beginCodePoint 문자 텍스처 아틀라스의 코드 포인트 시작점입니다.
	 * @param endCodePoint 문자 텍스처 아틀라스의 코드 포인트 끝점입니다.
	 * @param fontSize 폰트의 크기입니다.
	 * @param outGlyphs[out] 텍스처 아틀라스 상의 코드 포인트에 대응하는 문자 위치 및 크기 정보입니다.
	 * @param outAtlasSize[out] 텍스처 아틀라스 크기입니다.
	 *
	 * @return 생성된 텍스처 아틀라스 비트맵을 반환합니다.
	 */
	std::shared_ptr<uint8_t[]> GenerateTextureAtlasBitmap(
		const std::vector<uint8_t>& buffer,
		int32_t beginCodePoint,
		int32_t endCodePoint,
		float fontSize,
		std::vector<Glyph>& outGlyphs,
		int32_t& outAtlasSize
	);


	/**
	 * @brief 텍스처 아틀라스 비트맵으로 부터 텍스처 리소스를 생성합니다.
	 *
	 * @param device 텍스처 리소스를 생성할 때 사용할 디바이스입니다.
	 * @param bitmap 텍스처 아틀라스 비트맵입니다.
	 * @param atlasSize 텍스처 아틀라스 비트맵의 가로 세로 크기입니다
	 *
	 * @throws 텍스처 리소스 생성에 실패하면 C++ 표준 예외를 던집니다.
	 *
	 * @return 텍스처 아틀라스 생성 결과를 반환합니다. 생성에 성공하면 S_OK, 그렇지 않다면 그 외의 값을 반환합니다.
	 */
	HRESULT CreateTextureAtlasFromBitmap(
		ID3D11Device* device,
		const std::shared_ptr<uint8_t[]>& bitmap,
		const int32_t& atlasSize
	);


private:
	/**
	 * @brief 문자 텍스처 아틀라스의 코드 포인트 시작점입니다.
	 *
	 * @note 텍스처 아틀라스는 시작점을 포함합니다.
	 */
	int32_t beginCodePoint_ = 0;


	/**
	 * @brief 문자 텍스처 아틀라스의 코드 포인트 끝점입니다.
	 *
	 * @note 텍스처 아틀라스는 끝점을 포함합니다.
	 */
	int32_t endCodePoint_ = 0;


	/**
	 * @brief 텍스처 아틀라스의 크기입니다.
	 */
	int32_t atlasSize_ = 0;


	/**
	 * @brief 텍스처 아틀라스 상의 코드 포인트에 대응하는 문자 위치 및 크기 정보입니다.
	 */
	std::vector<Glyph> glyphs_;


	/**
	 * @brief 텍스트 텍스처 아틀라스입니다.
	 */
	ID3D11Texture2D* textureAtlas_ = nullptr;


	/**
	 * @brief 텍스트 텍스처 아틀라스 리소스 뷰입니다.
	 */
	ID3D11ShaderResourceView* textureAtlasView_ = nullptr;
};