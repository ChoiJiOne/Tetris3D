#pragma once

#include <directxmath.h>
#include <directxcollision.h>
#include <string>


/**
 * @brief 게임 내 블럭입니다.
 */
class Block
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
	 * @brief 게임 내 블럭의 기본 생성자입니다.
	 * 
	 * @note 이 생성자를 통해 객체가 생성되면 내부 요소의 적절한 초기화를 수행한 뒤에 사용해야 합니다.
	 */
	Block() = default;

	
	/**
	 * @brief 게임 내 블럭의 복사 생성자입니다.
	 *
	 * @param instance 복사할 블럭 인스턴스입니다.
	 */
	Block(Block&& instance) noexcept
		: position_(instance.position_)
		, size_(instance.size_)
		, boundingBox_(instance.boundingBox_)
		, color_(instance.color_) {}


	/**
	 * @brief 게임 내 블럭의 복사 생성자입니다.
	 * 
	 * @param instance 복사할 블럭 인스턴스입니다.
	 */
	Block(const Block& instance)
		: position_(instance.position_)
		, size_(instance.size_)
		, boundingBox_(instance.boundingBox_)
		, color_(instance.color_) {}

	
	/**
	 * @brief 게임 내 블럭 생성자입니다.
	 * 
	 * @note 게임 내 블럭은 한 변의 길이가 모두 같은 정육면체입니다.
	 * 
	 * @param position 월드 상 블럭의 위치입니다.
	 * @param size 블럭의 크기입니다.
	 * @param color 월드 상 블럭의 텍스처 색상 타입입니다.
	 */
	Block(const DirectX::XMFLOAT3& position, float size, const EColor& color);


	/**
	 * @brief 게임 내 블럭 오브젝트의 가상 소멸자입니다.
	 */
	virtual ~Block();


	/**
	 * @brief 게임 내 블럭의 대입 연산자입니다.
	 * 
	 * @param instance 대입할 블럭 인스턴스입니다.
	 * 
	 * @return 대입한 블럭 객체의 참조자를 반환합니다.
	 */
	Block& operator=(Block&& instance) noexcept
	{
		if (this == &instance) return *this;

		position_ = instance.position_;
		size_ = instance.size_;
		boundingBox_ = instance.boundingBox_;
		color_ = instance.color_;
		
		return *this;
	}


	/**
	 * @brief 게임 내 블럭의 대입 연산자입니다.
	 *
	 * @param instance 대입할 블럭 인스턴스입니다.
	 *
	 * @return 대입한 블럭 객체의 참조자를 반환합니다.
	 */
	Block& operator=(const Block& instance)
	{
		if (this == &instance) return *this;

		position_ = instance.position_;
		size_ = instance.size_;
		boundingBox_ = instance.boundingBox_;
		color_ = instance.color_;

		return *this;
	}

	
	/**
	 * @brief 블럭의 월드 상 위치를 얻습니다.
	 * 
	 * @return 블럭의 월드 상 위치를 반환합니다.
	 */
	DirectX::XMFLOAT3 GetPosition() const { return position_; }


	/**
	 * @brief 블럭의 크기를 얻습니다.
	 * 
	 * @return 블럭의 크기를 반환합니다.
	 */
	float GetSize() const { return size_; }


	/**
	 * @brief 블럭의 색상을 얻습니다.
	 * 
	 * @return 블럭의 색상을 반환합니다.
	 */
	EColor GetColor() const { return color_; }


	/**
	 * @brief 블럭의 월드 상 위치를 설정합니다.
	 * 
	 * @param position 설정할 월드 상 위치입니다.
	 */
	void SetPosition(const DirectX::XMFLOAT3& position);


	/**
	 * @brief 다른 블럭과 충돌을 검사합니다.
	 * 
	 * @param otherBlock 충돌을 검사할 다른 블럭입니다.
	 * 
	 * @return 다른 블럭과 충돌한다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool IsCollision(const Block& otherBlock) const;


	/**
	 * @brief 다른 블럭과 겹치는지 검사합니다.
	 * 
	 * @param otherBlock 겹치는지 검사할 다른 블럭입니다.
	 * 
	 * @return 다른 블럭과 겹친다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool IsOverlap(const Block& otherBlock) const;


	/**
	 * @brief 블럭의 색상 타입에 대응하는 텍스처의 시그니처 값을 얻습니다.
	 *
	 * @param color 블럭의 색상 형식입니다.
	 * 
	 * @return 블럭의 색상 타입에 대응하는 텍스처의 시그니처 값을 반환합니다.
	 */
	static const std::string& GetColorTextureSignature(const EColor& color);

	
private:
	/**
	 * @brief 블럭의 월드 상 위치입니다.
	 */
	DirectX::XMFLOAT3 position_;


	/**
	 * @brief 블럭의 크기입니다.
	 */
	float size_ = 0.0f;


	/**
	 * @brief 블럭의 경계 상자입니다.
	 */
	DirectX::BoundingBox boundingBox_;


	/**
	 * @brief 블럭의 색상 타입입니다.
	 */
	EColor color_ = EColor::NONE;
};