#pragma once

#include <d3d11.h>
#include <directxmath.h>

#include "Macro.h"


/**
 * @brief 특수 효과 없이 정점의 색상만 렌더링하는 셰이더입니다.
 */
class ColorNoEffectShader
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
	ColorNoEffectShader(ID3D11Device* device, const std::wstring& vertexShaderSourcePath, const std::wstring& pixelShaderSourcePath);


	/**
	 * @brief 특수 효과 없이 정점의 색상만 렌더링하는 셰이더의 가상 소멸자입니다.
	 */
	virtual ~ColorNoEffectShader();


	/**
	 * @brief 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(ColorNoEffectShader);


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
	 * @brief 셰이더를 파이프라인에 바인딩합니다.
	 * 
	 * @param context 렌더링을 수행할 컨텍스트입니다.
	 */
	void Bind(ID3D11DeviceContext* context);


private:
	/**
	 * @brief 셰이더를 컴파일합니다.
	 *
	 * @see https://learn.microsoft.com/en-us/windows/win32/api/d3dcompiler/nf-d3dcompiler-d3dcompilefromfile
	 *
	 * @note https://github.com/walbourn/directx-sdk-samples/blob/main/Direct3D11Tutorials/Tutorial07/Tutorial07.cpp#L177
	 *
	 * @param sourcePath 셰이더 소스 파일의 경로입니다.
	 * @param entryPoint 셰이더의 진입점입니다.
	 * @param shaderModel 셰이더의 기능 집합입니다.
	 * @param blob[out] 컴파일된 셰이더 리소스입니다.
	 *
	 * @return 셰이더 컴파일 결과를 반환합니다. 컴파일에 성공했다면 S_OK, 그렇지 않다면 그 이외의 값을 반환합니다.
	 */
	HRESULT CompileShaderFromFile(
		const std::wstring& sourcePath, 
		const std::string& entryPoint, 
		const std::string& shaderModel, 
		ID3DBlob** blob
	);


	/**
	 * @brief 정점 셰이더 소스로부터 정점 셰이더 리소스를 생성합니다.
	 *
	 * @param device 셰이더 생성에 필요한 디바이스입니다.
	 * @param sourcePath 정점 셰이더 소스의 경로입니다.
	 * @param vertexShaderSource[out] 컴파일된 정점 셰이더 소스입니다.
	 * @param vertexShader[out] 정점 셰이더 리소스입니다.
	 *
	 * @return 셰이더 리소스 생성 결과를 반환합니다. 생성에 성공했다면 S_OK, 그렇지 않다면 그 이외의 값을 반환합니다.
	 */
	HRESULT CreateVertexShaderFromFile(
		ID3D11Device* device,
		const std::wstring& sourcePath,
		ID3DBlob** vertexShaderSource,
		ID3D11VertexShader** vertexShader
	);


	/**
	 * @brief 픽셀 셰이더 소스로부터 정점 셰이더 리소스를 생성합니다.
	 *
	 * @param device 셰이더 생성에 필요한 디바이스입니다.
	 * @param sourcePath 픽셀 셰이더 소스의 경로입니다.
	 * @param pixelShaderSource[out] 컴파일된 픽셀 셰이더 소스입니다.
	 * @parma pixelShader[out] 픽셀 셰이더 리소스입니다.
	 *
	 * @return 셰이더 리소스 생성 결과를 반환합니다. 생성에 성공했다면 S_OK, 그렇지 않다면 그 이외의 값을 반환합니다.
	 */
	HRESULT CreatePixelShaderFromFile(
		ID3D11Device* device,
		const std::wstring& sourcePath,
		ID3DBlob** pixelShaderSource,
		ID3D11PixelShader** pixelShader
	);


	/**
	 * @brief 정점 셰이더에 전달할 정점 데이터를 생성합니다.
	 *
	 * @note 이 메서드를 호출하기 전에 정점 셰이더가 컴파일 되어야 합니다.
	 *
	 * @param device 정점 데이터를 생성할 때 사용할 디바이스입니다.
	 * @param vertexShaderSource 정점 데이터를 생성할 때 참고할 컴파일된 정점 셰이더의 소스입니다.
	 * @param inputLayoutElements 정점 셰이더에 전달할 정점 데이터 정보입니다.
	 * @param inputLayout[out] 생성된 정점 데이터 정보 리스소입니다.
	 *
	 * @return 정점 데이터 생성 결과를 반환합니다. 생성에 성공하면 S_OK, 그렇지 않다면 그 이외의 값을 반환합니다.
	 */
	HRESULT CreateInputLayout(
		ID3D11Device* device,
		ID3DBlob* vertexShaderSource,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutElements,
		ID3D11InputLayout** inputLayout
	);


private:
	/**
	 * @brief 정점 셰이더 소스 리소스입니다.
	 */
	ID3DBlob* vertexShaderSource_ = nullptr;


	/**
	 * @brief 정점 셰이더입니다.
	 */
	ID3D11VertexShader* vertexShader_ = nullptr;


	/**
	 * @brief 픽셀 셰이더 소스 리소스입니다.
	 */
	ID3DBlob* pixelShaderSource_ = nullptr;


	/**
	 * @brief 픽셀 셰이더입니다.
	 */
	ID3D11PixelShader* pixelShader_ = nullptr;


	/**
	 * @brief 렌더링 파이프라인에 바인딩될 정점 데이터에 대한 정의입니다.
	 * 
	 * @note https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nn-d3d11-id3d11inputlayout
	 */
	ID3D11InputLayout* inputLayout_ = nullptr;


	/**
	 * @brief 매 프레임 변경되는 셰이더 내의 리소스입니다.
	 */
	EveryFrameConstantBuffer everyFrameBufferResource_;


	/**
	 * @brief 매 프레임 변경되는 셰이더 내의 상수 버퍼입니다.
	 */
	ID3D11Buffer* everyFrameBuffer_ = nullptr;
};