#pragma once

#include <directxmath.h>
#include <functional>

#include "GameObject.h"

class FixCamera;
class SpriteNoEffectShader;
class Texture2D;


/**
 * @brief 게임 내 UI 버튼입니다.
 */
class Button : public GameObject
{
public:
	/**
	 * @brief UI 버튼 생성자 파라미터의 구조체입니다.
	 */
	struct ConstructorParam
	{
		int32_t updateOrder;
		bool bIsActive;
		DirectX::XMFLOAT2 relativeCenter;
		float width;
		float height;
		std::string textureSignature;
		float mouseOverTransparency;
		float mouseNotOverTransparency;
		float pressReduceRatio;
		std::function<void()> eventAction;
	};


public:
	/**
	 * @brief 게임 내 UI 버튼의 생성자입니다.
	 * 
	 * @param constructorParam UI 버튼의 생성자 파라미터입니다.
	 */
	Button(ConstructorParam&& constructorParam);


	/**
	 * @brief 게임 내 UI 버튼의 생성자입니다.
	 *
	 * @param constructorParam UI 버튼의 생성자 파라미터입니다.
	 */
	Button(const ConstructorParam& constructorParam);


	/**
	 * @brief 게임 내 UI 버튼의 생성자입니다.
	 * 
	 * @param updateOrder UI 버튼의 업데이트 순위입니다.
	 * @param bIsActive UI 버튼의 활성화 여부입니다.
	 * @param relativeCenter -1.0 ~ 1.0 범위의 UI 버튼의 중심 좌표입니다.
	 * @param width UI 버튼의 가로 크기입니다.
	 * @param height UI 버튼의 세로 크기입니다.
	 * @param textureSignature UI 버튼의 텍스처 시그니처입니다.
	 * @param mouseOverTransparency UI 버튼 위에 마우스가 올라왔을 때의 텍스처 투명도입니다.
	 * @param mouseNotOverTransparency UI 버튼 위에 마우스가 올라오지 않았을 때의 텍스처 투명도입니다.
	 * @param pressReduceRatio UI 버튼을 클릭했을 때의 줄어들 비율입니다.
	 * @param eventAction UI 버튼을 클릭했을 때 실행할 이벤트입니다.
	 */
	Button(
		int32_t updateOrder, 
		bool bIsActive,
		const DirectX::XMFLOAT2& relativeCenter,
		float width,
		float height,
		const std::string& textureSignature,
		float mouseOverTransparency,
		float mouseNotOverTransparency,
		float pressReduceRatio,
		const std::function<void()>& eventAction
	);

	
	/**
	 * @brief 게임 내 UI 버튼의 가상 소멸자입니다.
	 */
	virtual ~Button();


	/**
	 * @brief 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Button);


	/**
	 * @brief UI 버튼을 업데이트합니다.
	 * 
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


protected:
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


	/**
	 * @brief UI 버튼의 스크린 위치를 윈도우 위치로 변환합니다.
	 * 
	 * @param screenPosition UI 버튼의 스크린 좌표입니다.
	 * @param windowWidth 윈도우의 가로 크기입니다.
	 * @param windowHeight 윈도우의 세로 크기입니다.
	 * 
	 * @return 윈도우 좌표계 기준의 좌표를 반환합니다.
	 */
	DirectX::XMFLOAT2 ConvertScreenToWindowPosition(const DirectX::XMFLOAT2& screenPosition, float windowWidth, float windowHeight) const;
	

	/**
	 * @brief 마우스가 UI 버튼 위에 있는지 확인합니다.
	 * 
	 * @return 마우스가 UI 버튼 위에 있다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool IsMouseOverButton() const;


	/**
	 * @brief UI 버튼을 화면에 그립니다.
	 * 
	 * @param texture UI 버튼의 텍스처입니다.
	 * @param center UI 버튼의 스크린 상의 좌표입니다.
	 * @param width UI 버튼의 가로 크기입니다.
	 * @param height UI 버튼의 세로 크기입니다.
	 * @param transparency UI 버튼의 투명도입니다.
	 */
	void DrawUITexture(Texture2D* texture, const DirectX::XMFLOAT2& center, float width, float height, float transparency);


protected:
	/**  
	 * @brief UI 버튼의 스크린 상의 정규화된 중심 좌표입니다.
	 * 
	 * @note (x, y) 값의 범위가 -1.0 ~ 1.0입니다.
	 */ 
	DirectX::XMFLOAT2 relativeCenter_;


	/**
	 * @brief UI 버튼의 스크린 상의 중심 좌표입니다.
	 */
	DirectX::XMFLOAT2 centerInScreen_;


	/**
	 * @brief UI 버튼의 윈도우 상의 중심 좌표입니다.
	 */
	DirectX::XMFLOAT2 centerInWindow_;


	/**
	 * @brief UI 버튼의 가로 크기입니다.
	 */
	float width_ = 0.0f;


	/**
	 * @brief UI 버튼의 세로 크기입니다.
	 */
	float height_ = 0.0f;


	/**
	 * @brief UI 버튼의 텍스처입니다.
	 */
	Texture2D* texture_ = nullptr;


	/**
	 * @brief 마우스가 UI 버튼 위에 있는지 확인합니다.
	 */
	bool bIsMouseOverButton_ = false;

	
	/**
	 * @brief 마우스가 UI 버튼 위에 없을 때의 투명도입니다.
	 */
	float mouseNotOverTransparency_ = 1.0f;


	/**
	 * @brief 마우스가 UI 버튼 위에 있을 때의 투명도입니다.
	 */
	float mouseOverTransparency_ = 1.0f;


	/**
	 * @brief UI 버튼 클릭 시 줄어들 텍스처입니다.
	 */
	float pressReduceRatio_ = 1.0f;


	/**
	 * @brief UI 버튼 클릭 시 동작할 이벤트입니다.
	 */
	std::function<void()> eventAction_;
};