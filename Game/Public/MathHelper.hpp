#pragma once

#include <cmath>
#include <chrono>
#include <directxcollision.h>
#include <directxmath.h>
#include <numeric>
#include <random>


/**
 * @brief float 타입의 타입의 엡실론(ε) 값입니다.
 */
const float EPSILON_F = std::numeric_limits<float>::epsilon();


/**
 * @brief double 타입의 엡실론(ε) 값입니다.
 */
const double EPSILON_D = std::numeric_limits<double>::epsilon();


/**
 * @brief 수학 관련 처리에 대한 핼퍼 기능을 수행합니다.
 * 
 * @note 이 클래스는 멤버 함수 모두가 정적인 정적(static) 클래스입니다.
 */
class MathHelper
{
public:
	/**
	 * @brief 부동 소수점 값이 0인지 확인합니다.
	 * 
	 * @param value 0인지 확인할 부동 소수점 값입니다.
	 * 
	 * @return 부동 소수점 값이 0으로 판정되면 true, 그렇지 않으면 false를 반환합니다.
	 */
	static inline bool IsZero(float value, float epsilon = EPSILON_F)
	{
		return std::fabs(value) < epsilon;
	}


	/**
	 * @brief 부동 소수점 값이 0인지 확인합니다.
	 *
	 * @param value 0인지 확인할 부동 소수점 값입니다.
	 *
	 * @return 부동 소수점 값이 0으로 판정되면 true, 그렇지 않으면 false를 반환합니다.
	 */
	static inline bool IsZero(double value, double epsilon = EPSILON_D)
	{
		return std::abs(value) < epsilon;
	}


	/**
	 * @brief 임의의 정수를 생성합니다.
	 *
	 * @param minValue 생성할 난수 범위의 최솟값입니다.
	 * @param maxValue 생성할 난수 범위의 최댓값입니다.
	 *
	 * @return 생성된 임의의 정수를 반환합니다.
	 */
	static inline int32_t GenerateRandomInt(int32_t minValue, int32_t maxValue)
	{
		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_int_distribution<int32_t> distribution(std::min<int32_t>(minValue, maxValue), std::max<int32_t>(minValue, maxValue));

		return distribution(generator);
	}


	/**
	 * @brief 임의의 실수를 생성합니다.
	 *
	 * @param minValue 생성할 난수 범위의 최솟값입니다.
	 * @param maxValue 생성할 난수 범위의 최댓값입니다.
	 *
	 * @return 생성된 임의의 실수를 반환합니다.
	 */
	static inline float GenerateRandomFloat(float minValue, float maxValue)
	{
		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_real_distribution<float> distribution(std::min<float>(minValue, maxValue), std::max<float>(minValue, maxValue));

		return distribution(generator);
	}


	/**
	 * @brief 2차원 부동 소수점 벡터의 원소들을 반올림합니다.
	 * 
	 * @param float2 모든 원소를 반올림할 벡터입니다.
	 * 
	 * @return 모든 원소가 반올림된 새로운 벡터를 반환합니다.
	 */
	static inline DirectX::XMFLOAT2 Round(const DirectX::XMFLOAT2& float2)
	{
		return DirectX::XMFLOAT2(
			std::round(float2.x),
			std::round(float2.y)
		);
	}


	/**
	 * @brief 3차원 부동 소수점 벡터의 원소들을 반올림합니다.
	 *
	 * @param float3 모든 원소를 반올림할 벡터입니다.
	 *
	 * @return 모든 원소가 반올림된 새로운 벡터를 반환합니다.
	 */
	static inline DirectX::XMFLOAT3 Round(const DirectX::XMFLOAT3& float3)
	{
		return DirectX::XMFLOAT3(
			std::round(float3.x),
			std::round(float3.y),
			std::round(float3.z)
		);
	}


	/**
	 * @brief 4차원 부동 소수점 벡터의 원소들을 반올림합니다.
	 *
	 * @param float4 모든 원소를 반올림할 벡터입니다.
	 *
	 * @return 모든 원소가 반올림된 새로운 벡터를 반환합니다.
	 */
	static inline DirectX::XMFLOAT4 Round(const DirectX::XMFLOAT4& float4)
	{
		return DirectX::XMFLOAT4(
			std::round(float4.x),
			std::round(float4.y),
			std::round(float4.z),
			std::round(float4.w)
		);
	}
};