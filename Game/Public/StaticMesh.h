#pragma once

#include <d3d11.h>
#include <vector>

#include "Vertex.h"
#include "Macro.h"


/**
 * @brief 게임 월드 내의 정적 메시입니다.
 */
class StaticMesh
{
public:
	/**
	 * @brief 게임 월드 내의 정적 메시 클래스의 생성자입니다.
	 * 
	 * @param device 버퍼를 생성할 때 사용할 디바이스입니다.
	 * @param vertices 정적 메시의 정점 목록입니다.
	 * @param indices 정적 메시의 인덱스 목록입니다.
	 * 
	 * @throws 정점 생성에 실패하면 C++ 표준 예외를 던집니다.
	 */
	StaticMesh(ID3D11Device* device, const std::vector<Vertex::PositionColor>& vertices, const std::vector<uint32_t>& indices);


	/**
	 * @brief 게임 월드 내의 정적 메시 클래스의 가상 소멸자입니다.
	 */
	virtual ~StaticMesh();


	/**
	 * @brief 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(StaticMesh);


	/**
	 * @brief 정적 메시를 화면에 그립니다.
	 * 
	 * @param context 렌더링을 수행할 컨텍스트입니다.
	 */
	void Draw(ID3D11DeviceContext* context);


private:
	/**
	 * @brief 정점 버퍼를 생성합니다.
	 * 
	 * @param device 버퍼 생성에 사용할 디바이스입니다.
	 * @param byteWidth 정점 버퍼의 바이트 크기입니다.
	 * @param bufferPtr 정점 버퍼의 포인터입니다.
	 * @param outVertexBuffer[out] 생성된 정점 버퍼입니다.
	 * 
	 * @return 정점 버퍼 생성 결과를 반환합니다. 생성에 성공하면 S_OK, 그렇지 않다면 그 이외의 값을 반환합니다.
	 */
	HRESULT CreateVertexBuffer(ID3D11Device* device, uint32_t byteWidth, const void* bufferPtr, ID3D11Buffer** outVertexBuffer);


	/**
	 * @brief 인덱스 버퍼를 생성합니다.
	 * 
	 * @param device 버퍼 생성에 사용할 디바이스입니다.
	 * @param byteWidth 인덱스 버퍼의 바이트 크기입니다.
	 * @param bufferPtr 인덱스 버퍼의 포인터입니다.
	 * @param outIndexBuffer[out] 생성된 인덱스 버퍼입니다.
	 * 
	 * @return 인덱스 버퍼 생성 결과를 반환합니다. 생성에 성공하면 S_OK, 그렇지 않다면 그 이외의 값을 반환합니다.
	 */
	HRESULT CreateIndexBuffer(ID3D11Device* device, uint32_t byteWidth, const void* bufferPtr, ID3D11Buffer** outIndexBuffer);


private:
	/**
	 * @brief 정점 하나의 바이트 크기입니다.
	 */
	uint32_t vertexByteSize_ = 0;


	/**
	 * @brief 정적 메시의 정점 버퍼입니다.
	 */
	ID3D11Buffer* vertexBuffer_ = nullptr;


	/**
	 * @brief 인덱스의 수입니다.
	 */
	uint32_t countOfIndex_ = 0;


	/**
	 * @brief 정적 메시의 인덱스 버퍼입니다.
	 */
	ID3D11Buffer* indexBuffer_ = nullptr;
};