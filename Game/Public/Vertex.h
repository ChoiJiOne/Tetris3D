#pragma once

#include <directxmath.h>


/**
 * @brief 화면에 표시될 객체의 정점 정의 집합입니다.
 */
namespace Vertex
{
	/**
	 * @brief 위치와 색상 정보를 포함한 정점입니다.
	 */
	struct PositionColor
	{
		DirectX::XMFLOAT3 position; /** 정점의 위치입니다. */
		DirectX::XMFLOAT4 color;    /** 정점의 색상입니다. */
	};
}