#pragma once

#include <directxmath.h>

#include "EffectShader.h"
#include "Macro.h"

class Texture2D;


/**
 * @brief 특수 효과 없이 정점의 텍스처만 렌더링하는 셰이더입니다.
 */
class TextureNoEffectShader : public EffectShader
{
public:
	/**
	 * @brief 셰이더 내에 매 프레임 업데이트되는 상수 버퍼입니다.
	 */
	struct EveryFrameConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
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
	TextureNoEffectShader(ID3D11Device* device, const std::wstring& vertexShaderSourcePath, const std::wstring& pixelShaderSourcePath);


	/**
	 * @brief 특수 효과 없이 정점의 색상만 렌더링하는 셰이더의 가상 소멸자입니다.
	 */
	virtual ~TextureNoEffectShader();


	/**
	 * @brief 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(TextureNoEffectShader);


	/**
	 * @brief 월드 행렬을 설정합니다.
	 *
	 * @param world 설정할 월드 행렬입니다.
	 */
	void SetWorldMatrix(const DirectX::XMMATRIX& world)
	{
		everyFrameBufferResource_.world = DirectX::XMMatrixTranspose(world);
	}


	/**
	 * @brief 시야 행렬을 설정합니다.
	 *
	 * @param view 설정할 시야 행렬입니다.
	 */
	void SetViewMatrix(const DirectX::XMMATRIX& view)
	{
		everyFrameBufferResource_.view = DirectX::XMMatrixTranspose(view);
	}


	/**
	 * @brief 투영 행렬을 설정합니다.
	 *
	 * @param projection 설정할 투영 행렬입니다.
	 */
	void SetProjectionMatrix(const DirectX::XMMATRIX& projection)
	{
		everyFrameBufferResource_.projection = DirectX::XMMatrixTranspose(projection);
	}

	
	/**
	 * @brief 텍스처의 투명도를 설정합니다.
	 * 
	 * @param alpha 설정할 투명도 값입니다.
	 */
	void SetAlpha(float alpha)
	{
		transparentConstantBuffer.transparent.x = alpha;
		transparentConstantBuffer.transparent.y = alpha;
		transparentConstantBuffer.transparent.z = alpha;
		transparentConstantBuffer.transparent.w = alpha;
	}


	/**
	 * @brief 텍스처를 설정합니다.
	 * 
	 * @param texture 설정할 텍스처입니다.
	 */
	void SetTexture(Texture2D* texture);


	/**
	 * @brief 셰이더를 파이프라인에 바인딩합니다.
	 *
	 * @param context 렌더링을 수행할 컨텍스트입니다.
	 */
	virtual void Bind(ID3D11DeviceContext* context) override;


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
};