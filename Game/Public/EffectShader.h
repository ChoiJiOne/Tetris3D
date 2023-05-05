#pragma once

#include <d3d11.h>

#include "Macro.h"


/**
 * @brief 셰이더 파일을 로딩 및 컴파일하고 셰이더를 파이프라인에 바인딩합니다.
 */
class EffectShader
{
public:
	/**
	 * @brief 효과 셰이더의 디폴트 생성자입니다.
	 * 
	 * @note 이 클래스를 상속받은 하위 클래스에서 반드시 초기화해야 합니다.
	 */
	EffectShader() = default;


	/**
	 * @brief 효과 셰이더의 가상 소멸자입니다.
	 */
	virtual ~EffectShader();


	/**
	 * @brief 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(EffectShader);


	/**
	 * @brief 셰이더를 파이프라인에 바인딩합니다.
	 * 
	 * @param context 렌더링을 수행할 컨텍스트입니다.
	 */
	virtual void Bind(ID3D11DeviceContext* context);


protected:
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
	 * @param inputLayout[out] 생성된 정점 데이터 정보 리소스입니다.
	 *
	 * @return 정점 데이터 생성 결과를 반환합니다. 생성에 성공하면 S_OK, 그렇지 않다면 그 이외의 값을 반환합니다.
	 */
	HRESULT CreateInputLayout(
		ID3D11Device* device,
		ID3DBlob* vertexShaderSource,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutElements,
		ID3D11InputLayout** inputLayout
	);


	/**
	 * @brief 파이프라인에서 사용할 다이나믹 정점 버퍼를 생성합니다.
	 *
	 * @param device 버퍼를 생성할 때 사용할 디바이스입니다.
	 * @param vertices 정점 버퍼를 생성할 때 참조할 정점 목록입니다.
	 * @param outVertexBuffer[out] 생성된 정점 버퍼입니다.
	 *
	 * @return 버퍼 생성 결과를 반환합니다. 생성에 성공하면 S_OK, 그렇지 않으면 그 이외의 값을 반환합니다.
	 */
	template<typename VertexType>
	HRESULT CreateDynamicVertexBuffer(ID3D11Device* device, const std::vector<VertexType>& vertices, ID3D11Buffer** outVertexBuffer)
	{
		D3D11_BUFFER_DESC vertexBufferDesc = {};

		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * static_cast<uint32_t>(vertices.size());
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = reinterpret_cast<const void*>(&vertices[0]);
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		return Device->CreateBuffer(&vertexBufferDesc, &vertexData, outVertexBuffer);
	}


	/**
	 * @brief 파이프라인에서 사용할 인덱스 버퍼를 생성합니다.
	 *
	 * @param device 버퍼를 생성할 때 사용할 디바이스입니다.
	 * @param vertices 인덱스 버퍼를 생성할 때 참조할 인덱스 목록입니다.
	 * @param outIndexBuffer[out] 생성된 인덱스 버퍼입니다.
	 *
	 * @return 버퍼 생성 결과를 반환합니다. 생성에 성공하면 S_OK, 그렇지 않으면 그 이외의 값을 반환합니다.
	 */
	HRESULT CreateIndexBuffer(ID3D11Device* device, const std::vector<uint32_t>& indices, ID3D11Buffer** outIndexBuffer);


	/**
	 * @brief CPU에서 쓰기 가능한 동적 상수 버퍼를 생성합니다.
	 * 
	 * @param device 버퍼를 생성할 때 사용할 디바이스입니다.
	 * @param outConstantBuffer[out] 생성한 동적 상수 버퍼입니다.
	 * 
	 * @return 동적 버퍼 생성 결과를 반환합니다. 생성에 성공하면 S_OK, 그렇지 않다면 그 이외의 값을 반환합니다.
	 */
	template <typename BufferType>
	HRESULT CreateDynamicConstantBuffer(ID3D11Device* device, ID3D11Buffer** outConstantBuffer)
	{
		D3D11_BUFFER_DESC dynamicConstantBufferDesc = {};

		dynamicConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		dynamicConstantBufferDesc.ByteWidth = sizeof(BufferType);
		dynamicConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		dynamicConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		dynamicConstantBufferDesc.MiscFlags = 0;
		dynamicConstantBufferDesc.StructureByteStride = 0;

		return device->CreateBuffer(&dynamicConstantBufferDesc, nullptr, outConstantBuffer);
	}
		

protected:
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
};