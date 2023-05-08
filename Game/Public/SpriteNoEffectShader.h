#pragma once

#include <directxmath.h>

#include "EffectShader.h"
#include "Macro.h"
#include "Vertex.h"

class Texture2D;


/**
 * @brief 특수 효과 없이 2D 스프라이트를 렌더링하는 셰이더입니다.
 */
class SpriteNoEffectShader : public EffectShader
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
	 * @brief 셰이더 내에 투명도를 설정하기 위한 상수 버퍼입니다.
	 */
	struct TransparentConstantBuffer
	{
		DirectX::XMFLOAT4 transparent;
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
	SpriteNoEffectShader(ID3D11Device* device, const std::wstring& vertexShaderSourcePath, const std::wstring& pixelShaderSourcePath);


	/**
	 * @brief 특수 효과 없이 스프라이트만 렌더링하는 셰이더의 가상 소멸자입니다.
	 */
	virtual ~SpriteNoEffectShader();


	/**
	 * @brief 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(SpriteNoEffectShader);


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
	 * @brief 백버퍼에 스프라이트를 그립니다.
	 *
	 * @param context 렌더링을 수행할 컨텍스트입니다.
	 * @param texture 스프라이트를 렌더링할 때 참조할 텍스처입니다.
	 * @param center 스크린 상의 스프라이트 중심 좌표입니다.
	 * @param width 스프라이트의 가로 크기입니다.
	 * @param height 스프라이트의 세로 크기입니다.
	 * @param alpha 스프라이트의 투명도입니다. 기본 값은 1.0입니다.
	 */
	void DrawSprite2D(ID3D11DeviceContext* context, Texture2D* texture, const DirectX::XMFLOAT2& center, float width, float height, float alpha = 1.0f);


private:
	/**
	 * @brief 정점 버퍼를 스프라이트 크기에 맞게 업데이트합니다.
	 * 
	 * @param context 버퍼를 업데이트할 때 사용할 컨텍스트입니다.
	 * @param center 스크린 상의 스프라이트 중심 좌표입니다.
	 * @param width 스크린 상의 스프라이트 가로 크기입니다.
	 * @param height 스크린 상의 스프라이트 세로 크기입니다.
	 */
	void UpdateVertexBuffer(ID3D11DeviceContext* context, const DirectX::XMFLOAT2& center, float width, float height);

	
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
	 * @brief 투명도를 결정하는 셰이더 내 상수 버퍼의 바인딩 슬롯입니다.
	 */
	uint32_t transparentBufferBindSlot_ = 0;


	/**
	 * @brief 투명도를 결정하는 셰이더 내의 리소스입니다.
	 */
	TransparentConstantBuffer transparentConstantBuffer;


	/**
	 * @brief 투명도를 결정하는 셰이더 내의 상수 버퍼입니다.
	 */
	ID3D11Buffer* transparentBuffer_ = nullptr;


	/**
	 * @brief 텍스처 샘플러의 바인딩 슬롯입니다.
	 */
	uint32_t samplerStateBindSlot_ = 0;


	/**
	 * @brief 텍스처 샘플링을 위한 샘플러 상태입니다.
	 */
	ID3D11SamplerState* linearSamplerState_ = nullptr;


	/**
	 * @brief 렌더링할 텍스처 리소스입니다.
	 */
	ID3D11ShaderResourceView* textureResource_ = nullptr;


	/**
	 * @brief 텍스처 리소스의 바인딩 위치입니다.
	 */
	uint32_t textureResourcebindSlot_ = 0;


	/**
	 * @brief 스프라이트의 정점 목록입니다.
	 */
	std::vector<Vertex::PositionUV> spriteVertex_;


	/**
	 * @brief 스프라이트의 정점 버퍼입니다.
	 */
	ID3D11Buffer* spriteVertexBuffer_ = nullptr;


	/**
	 * @brief 스프라이트의 인덱스 목록입니다.
	 */
	std::vector<uint32_t> spriteIndex_;


	/**
	 * @brief 스프라이트의 인덱스 버퍼입니다.
	 */
	ID3D11Buffer* spriteIndexBuffer_ = nullptr;
};