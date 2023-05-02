#pragma once

#include <cmath>
#include <directxcollision.h>
#include <directxmath.h>
#include <numeric>


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
	bool IsZero(float value, float epsilon = EPSILON_F)
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
	bool IsZero(double value, double epsilon = EPSILON_D)
	{
		return std::abs(value) < epsilon;
	}
};