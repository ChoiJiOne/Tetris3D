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
	 * @brief 정점과 텍스처 좌표가 포함된 정점의 박스를 생성합니다.
	 * 
	 * @param width 박스의 가로 크기입니다.
	 * @param height 박스의 세로 크기입니다.
	 * @param depth 박스의 깊이 크기입니다.
	 * @param outVertices[out]
	 * @param outIndices[out]
	 */
	inline static void CreateBox(float width, float height, float depth, std::vector<Vertex::PositionUV>& outVertices, std::vector<uint32_t>& outIndices);
};