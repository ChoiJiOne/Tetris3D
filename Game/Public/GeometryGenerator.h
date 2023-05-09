#pragma once

#include <vector>

#include "Vertex.h"


/**
 * @brief 정점 타입에 맞는 기하 도형을 생성합니다.
 * 
 * @note 이 클래스는 모든 멤버 변수 및 함수가 정적인 정적 클래스입니다.
 */
class GeometryGenerator
{
public:
	/**
	 * @brief 위치와 텍스처 좌표가 포함된 XY평면 쿼드를 생성합니다.
	 *
	 * @param width 쿼드의 가로 크기입니다.
	 * @param height 쿼드의 세로 크기입니다.
	 * @param outVertices[out] 생성된 쿼드의 정점 목록입니다.
	 * @param outIndices[out] 생성된 쿼드의 정점의 인덱스 목록입니다.
	 */
	static void CreateQuadXY(float width, float height, std::vector<Vertex::PositionUV>& outVertices, std::vector<uint32_t>& outIndices);


	/**
	 * @brief 위치와 텍스처 좌표가 포함된 YZ평면 쿼드를 생성합니다.
	 *
	 * @param width 쿼드의 가로 크기입니다.
	 * @param height 쿼드의 세로 크기입니다.
	 * @param outVertices[out] 생성된 쿼드의 정점 목록입니다.
	 * @param outIndices[out] 생성된 쿼드의 정점의 인덱스 목록입니다.
	 */
	static void CreateQuadYZ(float width, float height, std::vector<Vertex::PositionUV>& outVertices, std::vector<uint32_t>& outIndices);


	/**
	 * @brief 위치와 텍스처 좌표가 포함된 XZ평면 쿼드를 생성합니다.
	 *
	 * @param width 쿼드의 가로 크기입니다.
	 * @param height 쿼드의 세로 크기입니다.
	 * @param outVertices[out] 생성된 쿼드의 정점 목록입니다.
	 * @param outIndices[out] 생성된 쿼드의 정점의 인덱스 목록입니다.
	 */
	static void CreateQuadXZ(float width, float height, std::vector<Vertex::PositionUV>& outVertices, std::vector<uint32_t>& outIndices);

	
	/**
	 * @brief 정점과 텍스처 좌표가 포함된 정점의 박스를 생성합니다.
	 * 
	 * @param width 박스의 가로 크기입니다.
	 * @param height 박스의 세로 크기입니다.
	 * @param depth 박스의 깊이 크기입니다.
	 * @param outVertices[out] 생성된 박스의 정점 목록입니다.
	 * @param outIndices[out] 생성된 박스 정점의 인덱스 목록입니다.
	 */
	static void CreateBox(float width, float height, float depth, std::vector<Vertex::PositionUV>& outVertices, std::vector<uint32_t>& outIndices);
};