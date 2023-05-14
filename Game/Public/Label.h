#pragma once

#include <directxmath.h>

#include "GameObject.h"

class TTFont;


/**
 * @brief 게임 내 UI 라벨입니다.
 */
class Label : public GameObject
{
public:
	/**
	 * @brief UI 라벨 생성자 파라미터의 구조체입니다.
	 */
	struct ConstructorParam
	{
		int32_t updateOrder;
		bool bIsActive;
		DirectX::XMFLOAT2 relativeCenter;
		std::string fontSignature;
		std::wstring text;
		DirectX::XMFLOAT4 color;
	};

	
public:
	/**
	 * @brief 게임 내 UI 라벨의 생성자입니다.
	 *
	 * @param constructorParam UI 라벨의 생성자 파라미터입니다.
	 */
	Label(ConstructorParam&& constructorParam);


	/**
	 * @brief 게임 내 UI 라벨의 생성자입니다.
	 *
	 * @param constructorParam UI 라벨의 생성자 파라미터입니다.
	 */
	Label(const ConstructorParam& constructorParam);


	/**
	 * @brief 게임 내 UI 라벨의 생성자입니다.
	 * 
	 * @param updateOrder UI 라벨의 업데이트 순위입니다.
	 * @param bIsActive UI 라벨의 활성화 여부입니다.
	 * @param relativeCenter -1.0 ~ 1.0 범위의 UI 라벨의 중심 좌표입니다.
	 * @param fontSignature 텍스트 폰트의 시그니처입니다.
	 * @param text 라벨 내의 텍스트입니다.
	 * @param color 라벨 내 텍스트의 색상입니다.
	 */
	Label(
		int32_t updateOrder,
		bool bIsActive,
		const DirectX::XMFLOAT2& relativeCenter,
		const std::string& fontSignature,
		const std::wstring& text,
		const DirectX::XMFLOAT4& color
	);


	/**
	 * @brief 게임 내 UI 라벨의 가상 소멸자입니다.
	 */
	virtual ~Label();


	/**
	 * @brief 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Label);


	/**
	 * @brief UI 라벨을 업데이트합니다.
	 * 
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;

	
	/**
	 * @brief UI 라벨 내 텍스트를 설정합니다.
	 * 
	 * @param text UI 라벨 내 설정할 텍스트입니다.
	 */
	void SetText(const std::wstring& text) { text_ = text; }


	/**
	 * @brief UI 라벨 내 텍스트의 색상을 설정합니다.
	 * 
	 * @param color UI 라벨 내 설정할 RGBA 색상입니다.
	 */
	void SetColor(const DirectX::XMFLOAT4& color) { color_ = color; }


private:
	/**
	 * @brief UI 버튼의 상대 위치를 스크린 위치로 변환합니다.
	 *
	 * @param relativePosition -1.0 ~ 1.0 사이의 정규화된 좌표입니다.
	 * @param screenWidth 스크린의 가로 크기입니다.
	 * @param screenHeight 스크린의 세로 크기입니다.
	 *
	 * @return 스크린 좌표계 기준의 좌표를 반환합니다.
	 */
	DirectX::XMFLOAT2 ConvertRelativeToScreenPosition(const DirectX::XMFLOAT2& relativePosition, float screenWidth, float screenHeight) const;
	
	
private:
	/**
	 * @brief UI 라벨의 스크린 상의 정규화된 중심 좌표입니다.
	 *
	 * @note (x, y) 값의 범위가 -1.0 ~ 1.0입니다.
	 */
	DirectX::XMFLOAT2 relativeCenter_;


	/**
	 * @brief UI 라벨의 스크린 상의 중심 좌표입니다.
	 */
	DirectX::XMFLOAT2 centerInScreen_;


	/**
	 * @brief UI 라벨 내 텍스트의 폰트입니다.
	 */
	TTFont* font_ = nullptr;


	/**
	 * @brief UI 라벨 내 텍스트입니다.
	 */
	std::wstring text_;


	/**
	 * @brief UI 라벨 내 텍스트의 색상입니다.
	 */
	DirectX::XMFLOAT4 color_;
};