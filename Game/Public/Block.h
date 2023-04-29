#pragma once

#include <directxmath.h>
#include <directxcollision.h>

#include "GameObject.h"

class Texture2D;
class StaticMesh;


/**
 * @brief 게임 내 블럭 오브젝트입니다.
 */
class Block : public GameObject
{
public:
	/**
	 * @brief 블럭의 색상 타입입니다.
	 */
	enum class EColor
	{
		NONE    = 0x00,
		BLUE    = 0x01,
		CYAN    = 0x02,
		GRAY    = 0x03,
		GREEN   = 0x04,
		MAGENTA = 0x05,
		ORANGE  = 0x06,
		PINK    = 0x07,
		PURPLE  = 0x08,
		RED     = 0x09,
		YELLOW  = 0x0A
	};


public:
	/**
	 * @brief 게임 내 블럭 오브젝트의 생성자입니다.
	 * 
	 * @param position 월드 상 블럭의 위치입니다.
	 * @param color 월드 상 블럭의 텍스처 색상 타입입니다.
	 */
	Block(const DirectX::XMFLOAT3 position, const EColor& color);


	/**
	 * @brief 게임 내 블럭 오브젝트의 가상 소멸자입니다.
	 */
	virtual ~Block();


	/**
	 * @brief 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Block);


	/**
	 * @brief 블럭 오브젝트를 업데이트합니다.
	 * 
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;

	
private:
	/**
	 * @brief 블럭의 월드 상 위치입니다.
	 */
	DirectX::XMFLOAT3 position_;


	/**
	 * @brief 블럭의 경계 상자입니다.
	 */
	DirectX::BoundingBox boundingBox_;


	/**
	 * @brief 블럭의 텍스처 색상 타입입니다.
	 */
	EColor color_ = EColor::NONE;


	/**
	 * @brief 블럭의 정적 메시입니다.
	 */
	StaticMesh* staticMesh_ = nullptr;

	
	/**
	 * @brief 블럭의 텍스처입니다.
	 */
	Texture2D* texture_ = nullptr;
};