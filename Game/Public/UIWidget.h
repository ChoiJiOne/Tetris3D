#pragma once

#include <directxmath.h>

#include "GameObject.h"

class FixCamera;
class TTFont;
class Texture2D;


/**
 * @brief 게임 내 UI 위젯 입니다.
 */
class UIWidget : public GameObject
{
public:
	/**
	 * @brief UI 위젯 생성자 파라미터의 구조체입니다.
	 */
	struct ConstructorParam
	{
		int32_t updateOrder;
		bool bIsActive;
		DirectX::XMFLOAT2 center;
		float width;
		float height;
		std::string backgroundSignature;
		float backgroundTransparency;
		std::wstring text;
		std::string fontSignature;
		DirectX::XMFLOAT4 textColor;
	};


	/**
	 * @brief 백그라운드가 없는 UI 위젯 생성자 파라미터의 구조체입니다.
	 */
	struct ConstructorParamWithoutBackground
	{
		int32_t updateOrder;
		bool bIsActive;
		DirectX::XMFLOAT2 center;
		std::wstring text;
		std::string fontSignature;
		DirectX::XMFLOAT4 textColor;
	};

	
	/**
	 * @brief 텍스트가 없는 UI 위젯 생성자 파라미터의 구조체입니다.
	 */
	struct ConstructorParamWithoutText
	{
		int32_t updateOrder;
		bool bIsActive;
		DirectX::XMFLOAT2 center;
		float width;
		float height;
		std::string backgroundSignature;
		float backgroundTransparency;
	};


public:
	/**
	 * @brief UI 위젯의 생성자입니다.
	 * 
	 * @param constructorParam UI 위젯의 생성자 파라미터입니다.
	 */
	UIWidget(ConstructorParam&& constructorParam);


	/**
	 * @brief UI 위젯의 생성자입니다.
	 *
	 * @param constructorParam UI 위젯의 생성자 파라미터입니다.
	 */
	UIWidget(const ConstructorParam& constructorParam);


	/**
	 * @brief 백그라운드가 없는 UI 위젯의 생성자입니다.
	 * 
	 * @param constructorParam UI 위젯의 생성자 파라미터입니다.
	 */
	UIWidget(ConstructorParamWithoutBackground&& constructorParam);


	/**
	 * @brief 백그라운드가 없는 UI 위젯의 생성자입니다.
	 * 
	 * @param constructorParam UI 위젯의 생성자 파라미터입니다.
	 */
	UIWidget(const ConstructorParamWithoutBackground& constructorParam);


	/**
	 * @brief 텍스트가 없는 UI 위젯의 생성자입니다.
	 * 
	 * @param constructorParam UI 위젯의 생성자 파라미터입니다.
	 */
	UIWidget(ConstructorParamWithoutText&& constructorParam);


	/**
	 * @brief 텍스트가 없는 UI 위젯의 생성자입니다.
	 *
	 * @param constructorParam UI 위젯의 생성자 파라미터입니다.
	 */
	UIWidget(const ConstructorParamWithoutText& constructorParam);


	/**
	 * @brief UI 위젯의 생성자입니다.
	 * 
	 * @param updateOrder 게임 오브젝트의 업데이트 순위입니다.
	 * @param bIsActive UI 위젯의 활성화 여부입니다.
	 * @param center 스크린 상의 슬레이트 중심 좌표입니다.
	 * @param width 스크린 상의 슬레이트 가로 크기입니다.
	 * @param height 스크린 상의 슬레이트 세로 크기입니다.
	 * @param backgroundSignature 백그라운드 텍스처 리소스의 시그니처 값입니다.
	 * @param backgroundTransparency 백그라운드 텍스처의 투명도입니다.
	 * @param text 슬레이트 내의 텍스트입니다.
	 * @param fontSignature 슬레이트 내 텍스트 폰트 리소스의 시그니처입니다.
	 * @param textColor 텍스트의 색상입니다.
	 */
	UIWidget(
		int32_t updateOrder, 
		bool bIsActive,
		const DirectX::XMFLOAT2& center,
		float width,
		float height,
		const std::string& backgroundSignature,
		float backgroundTransparency,
		const std::wstring& text,
		const std::string& fontSignature,
		const DirectX::XMFLOAT4& textColor
	);


	/**
	 * @brief UI 위젯의 가상 소멸자입니다.
	 */
	virtual ~UIWidget();


	/**
	 * @brief 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(UIWidget);


	/**
	 * @brief UI 위젯를 업데이트합니다.
	 * 
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;
	

protected:
	/**
	 * @brief UI 위젯 중 백그라운드를 그립니다.
	 * 
	 * @param fixCamera 고정 카메라입니다.
	 */
	void DrawBackgroundUI(FixCamera* fixCamera);


	/**
	 * @brief UI 위젯 중 텍스트를 그립니다.
	 * 
	 * @param fixCamera 고정 카메라입니다.
	 */
	void DrawTextUI(FixCamera* fixCamera);


protected:
	/**
	 * @brief 스크린 상의 중심 좌표입니다.
	 */
	DirectX::XMFLOAT2 center_;
	

	/**
	 * @brief 스크린 상의 슬레이트 가로 크기입니다.
	 */
	float width_ = 0.0f;


	/**
	 * @brief 스크린 상의 슬레이트 세로 크기입니다.
	 */
	float height_ = 0.0f;


	/**
	 * @brief 슬레이트의 백그라운드 텍스처입니다.
	 */
	Texture2D* background_ = nullptr;


	/**
	 * @brief 백그라운드의 투명도입니다.
	 */
	float backgroundTransparency_ = 1.0f;


	/**
	 * @brief 슬레이트 내의 텍스트입니다.
	 */
	std::wstring text_;


	/**
	 * @brief 슬레이트 내에 텍스트의 폰트입니다.
	 */
	TTFont* font_ = nullptr;


	/**
	 * @brief 슬레이트 내에 텍스트 색상입니다.
	 */
	DirectX::XMFLOAT4 textColor_;
};