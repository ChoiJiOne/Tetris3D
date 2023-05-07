#pragma once

#include <directxmath.h>

#include "EffectShader.h"
#include "Macro.h"
#include "Vertex.h"

struct Glyph;
class TTFont;


/**
 * @brief 특수 효과 없이 텍스트를 렌더링하는 셰이더입니다.
 */
class TextNoEffectShader : public EffectShader
{
public:
	/**
	 * @brief 셰이더 내에 매 프레임 업데이트되는 상수 버퍼입니다.
	 */
	struct EveryFrameConstantBuffer
	{
		DirectX::XMMATRIX projection;
	};


	/**
	 * @brief 텍스트의 색상을 나타내는 상수 버퍼입니다.
	 */
	struct TextColorConstantBuffer
	{
		DirectX::XMFLOAT4 textColor;
	};


public:
    /**
	 * @brief 셰이더를 컴파일하고 정점 셰이더와 픽셀 셰이더를 생성합니다.
	 *
	 * @param device 셰이더 리소스를 생성할 때 사용할 디바이스입니다.
	 * @param vertexShaderSourcePath 정점 셰이더 소스 파일의 경로입니다.
	 * @param pixelShaderSourcePath 픽셀 셰이더 소스 파일의 경로입니다.
	 *
	 * @throws
	 * 셰이더 컴파일에 실패하면 C++ 표준 예외를 던집니다.
	 * 셰이더 리소스 생성에 실패하면 C++ 표준 예외를 던집니다.
	 */
	TextNoEffectShader(ID3D11Device* device, const std::wstring& vertexShaderSourcePath, const std::wstring& pixelShaderSourcePath);


	/**
	 * @brief 특수 효과 없이 텍스트만 렌더링하는 셰이더의 가상 소멸자입니다.
	 */
	virtual ~TextNoEffectShader();


	/**
	 * @brief 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(TextNoEffectShader);


	/**
	 * @brief 투영 행렬을 설정합니다.
	 *
	 * @param projection 설정할 투영 행렬입니다.
	 */
	void SetProjectionMatrix(const DirectX::XMMATRIX& projection)
	{
		everyFrameBufferResource_.projection = projection;
	}


	/**
	 * @brief 백 버퍼에 텍스트를 그립니다.
	 *
	 * @param context 렌더링을 수행할 컨텍스트입니다.
	 * @param font 텍스트를 렌더링할 때 참조할 폰트 리소스입니다.
	 * @param text 렌더링할 텍스트입니다.
	 * @param center 텍스처의 중심 좌표입니다.
	 * @param color 텍스의 색상입니다.
	 */
	void DrawText2D(
		ID3D11DeviceContext* context, 
		TTFont& font, 
		const std::wstring& text, 
		const DirectX::XMFLOAT2& center,
		const DirectX::XMFLOAT4& color
	);


private:
	/**
	 * @brief 정점 버퍼를 글리프에 맞게 업데이트합니다.
	 * 
	 * @param context 버퍼를 업데이트할 때 사용할 컨텍스트입니다.
	 * @param glyph 글리프입니다.
	 * @param atlasSize 텍스처 아틀라스의 크기입니다.
	 * @param position 스크린 상 유니 코드의 위치입니다.
	 */
	void UpdateVertexBuffer(ID3D11DeviceContext* context, const Glyph& glyph, float atlasSize, const DirectX::XMFLOAT2& position);

	
private:
	/**
	 * @brief 매 프레임 변경되는 셰이더 내 상수 버퍼의 바인딩 슬롯입니다.
	 */
	uint32_t everyFrameBufferBindSlot_ = 0;


	/**
	 * @brief 매 프레임 변경되는 셰이더 내의 리소스입니다.
	 */
	EveryFrameConstantBuffer everyFrameBufferResource_;


	/**
	 * @brief 매 프레임 변경되는 셰이더 내의 상수 버퍼입니다.
	 */
	ID3D11Buffer* everyFrameBuffer_ = nullptr;


	/**
	 * @brief 텍스트 색상을 나타내는 셰이더 내 상수 버퍼의 바인딩 슬롯입니다.
	 */
	uint32_t textColorBufferBindSlot_ = 0;


	/**
	 * @brief 텍스트 색상을 나타내는 셰이더 내의 리소스입니다.
	 */
	TextColorConstantBuffer textColorConstantBuffer;


	/**
	 * @brief 텍스트 색상을 나타내는 셰이더 내의 상수 버퍼입니다.
	 */
	ID3D11Buffer* textColorBuffer_ = nullptr;


	/**
	 * @brief 텍스처 샘플러의 바인딩 슬롯입니다.
	 */
	uint32_t samplerStateBindSlot_ = 0;


	/**
	 * @brief 텍스처 샘플링을 위한 샘플러 상태입니다.
	 */
	ID3D11SamplerState* linearSamplerState_ = nullptr;


	/**
	 * @brief 렌더링할 텍스처 아틀라스 리소스입니다.
	 */
	ID3D11ShaderResourceView* textureAtlasResource_ = nullptr;


	/**
	 * @brief 텍스처 아틀라스 리소스의 바인딩 위치입니다.
	 */
	uint32_t textureAtlasResourcebindSlot_ = 0;


	/**
	 * @brief 텍스트 내 문자의 정점 목록입니다.
	 */
	std::vector<Vertex::PositionUV> characterVertex_;


	/**
	 * @brief 텍스처의 정점 버퍼입니다.
	 */
	ID3D11Buffer* characterVertexBuffer_ = nullptr;


	/**
	 * @brief 텍스트 내 문자의 인덱스 목록입니다.
	 */
	std::vector<uint32_t> characterIndex_;


	/**
	 * @brief 텍스처의 인덱스 버퍼입니다.
	 */
	ID3D11Buffer* characterIndexBuffer_ = nullptr;
};