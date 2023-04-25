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
		/**
		 * @brief 위치와 색상 정보를 포함한 정점의 기본 생성자입니다.
		 */
		PositionColor() : position(0.0f, 0.0f, 0.0f), color(0.0f, 0.0f, 0.0f, 0.0f) {}


		/**
		 * @brief 위치와 색상 정보를 포함한 정점의 생성자입니다.
		 * 
		 * @param p 정점의 위치입니다.
		 * @param c 정점의 색상입니다.
		 */
		PositionColor(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT4& c)
			: position(p)
			, color(c) {}


		/**
		 * @brief 위치와 색상 정보를 포함한 정점의 복사 생성자입니다.
		 * 
		 * @param instance 복사할 정점의 인스턴스입니다.
		 */
		PositionColor(PositionColor&& instance) noexcept
			: position(instance.position)
			, color(instance.color) {}


		/**
		 * @brief 위치와 색상 정보를 포함한 정점의 복사 생성자입니다.
		 *
		 * @param instance 복사할 정점의 인스턴스입니다.
		 */
		PositionColor(const PositionColor& instance)
			: position(instance.position)
			, color(instance.color) {}


		/**
		 * @brief 위치와 색상 정보를 포함한 정점의 대입 연산자입니다.
		 * 
		 * @param instacne 대입할 정점의 인스턴스입니다.
		 * 
		 * @return 대입한 객체를 반환합니다.
		 */
		PositionColor& operator=(PositionColor&& instance) noexcept
		{
			if (this == &instance) return *this;

			position = instance.position;
			color = instance.color;

			return *this;
		}


		/**
		 * @brief 위치와 색상 정보를 포함한 정점의 대입 연산자입니다.
		 *
		 * @param instacne 대입할 정점의 인스턴스입니다.
		 *
		 * @return 대입한 객체를 반환합니다.
		 */
		PositionColor& operator=(const PositionColor& instance)
		{
			if (this == &instance) return *this;

			position = instance.position;
			color = instance.color;

			return *this;
		}


		/** 
		 * 정점의 위치입니다. 
		 */
		DirectX::XMFLOAT3 position;


		/**
		 * 정점의 색상입니다.
		 */
		DirectX::XMFLOAT4 color;
	};
}