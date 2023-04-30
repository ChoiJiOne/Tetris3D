#pragma once

#include <vector>

#include "Block.h"
#include "GameObject.h"

class StaticMesh;
class Texture2D;


/**
 * @brief 테트리스 게임의 테트로미노입니다.
 */
class Tetromino : public GameObject
{
public:
	/**
	 * @brief 테트로미노의 모양입니다.
	 */
	enum class EShape : int32_t
	{
		NONE = 0x00,
		I = 0x01,
		O = 0x02,
		T = 0x03,
		J = 0x04,
		L = 0x05,
		S = 0x06,
		Z = 0x07,
	};


public:
	/**
	 * @brief 테트로미노의 생성자입니다.
	 * 
	 * @param updateOrder 테트로미노의 업데이트 순위입니다.
	 * @param bIsActive 테트로미노의 활성화 여부입니다.
	 * @param blockSize 테트로미노 블럭의 크기입니다.
	 * @param blockColor 테트로미노 블럭의 색상입니다.
	 */
	Tetromino(
		int32_t updateOrder, bool bIsActive,
		float blockSize, const Block::EColor& blockColor
	);


	/**
	 * @brief 테트로미노의 가상 소멸자입니다.
	 */
	virtual ~Tetromino();


	/**
	 * @brief 복사 생성자 및 대입 연산자를 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Tetromino);


	/**
	 * @brief 테트로미노를 업데이트합니다.
	 * 
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


private:
	/**
	 * @brief 테트로미노의 모양입니다.
	 */
	EShape shape_ = EShape::NONE;


	/**
	 * @brief 테트로미노 내의 블럭입니다.
	 */
	std::vector<Block> blocks_;


	/**
	 * @brief 블럭의 정적 메시입니다.
	 */
	StaticMesh* blockStaticMesh_ = nullptr;


	/**
	 * @brief 블럭의 텍스처입니다.
	 */
	Texture2D* blockTexture_ = nullptr;
};