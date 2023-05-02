#pragma once

#include <vector>
#include <unordered_map>

#include "Block.h"
#include "GameObject.h"

class StaticMesh;
class Texture2D;


/**
 * @brief 테트리스 게임의 보드입니다.
 */
class Board : public GameObject
{
public:
	/**
	 * @brief 보드의 생성자입니다.
	 * 
	 * @param updateOrder 보드의 업데이트 순위입니다.
	 * @param bIsActive 보드의 활성화 여부입니다.
	 * @param basePosition 보드의 기준 좌표입니다.
	 * @param blockSize 보드 내 블럭의 크기입니다.
	 * @param blockColor 보드 내 블럭의 색상입니다.
	 * @param countRowBlock 보드 내 세로 블럭 수입니다.
	 * @param countColBlock 보드 내 가로 블럭 수입니다.
	 */
	Board(
		int32_t updateOrder,
		bool bIsActive,
		const DirectX::XMFLOAT3& basePosition,
		float blockSize,
		const Block::EColor& blockColor,
		int32_t countRowBlock,
		int32_t countColBlock
	);


	/**
	 * @brief 보드의 가상 소멸자입니다.
	 */
	virtual ~Board();


	/**
	 * @brief 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Board);


	/**
	 * @brief 보드를 업데이트합니다.
	 * 
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * @brief 보드의 외곽 라인 블럭을 얻습니다.
	 * 
	 * @return 외곽 라인 블럭의 목록을 반환합니다.
	 */
	const std::vector<Block>& GetOutlineBlocks() const { return outlineBlocks_; }


private:
	/**
	 * @brief 보드의 외곽선 블럭들을 생성합니다.
	 * 
	 * @param basePosition 보드의 기준 좌표입니다.
	 * @param blockSize 보드 내 블럭의 크기입니다.
	 * @param blockColor 보드 내 블럭의 색상입니다.
	 * @param countRowBlock 보드 내 세로 블럭 수입니다.
	 * @param countColBlock 보드 내 가로 블럭 수입니다.
	 * 
	 * @return 생성된 외곽선 블럭들을 반환합니다.
	 */
	std::vector<Block> GenerateOutlineBlocks(
		const DirectX::XMFLOAT3& basePosition,
		float blockSize,
		const Block::EColor& blockColor,
		int32_t countRowBlock,
		int32_t countColBlock
	);


	/**
	 * @brief 보드의 백그라운드 블럭들을 생성합니다.
	 *
	 * @param basePosition 보드의 기준 좌표입니다.
	 * @param blockSize 보드 내 블럭의 크기입니다.
	 * @param blockColor 보드 내 블럭의 색상입니다.
	 * @param countRowBlock 보드 내 세로 블럭 수입니다.
	 * @param countColBlock 보드 내 가로 블럭 수입니다.
	 * 
	 * @return 생성된 백그라운드 블럭들을 반환합니다.
	 */
	std::vector<Block> GenerateBackgroundBlocks(
		const DirectX::XMFLOAT3& basePosition,
		float blockSize,
		const Block::EColor& blockColor,
		int32_t countRowBlock,
		int32_t countColBlock
	);


	/**
	 * @brief 블럭들을 백버퍼에 그립니다.
	 * 
	 * @param blocks 백버퍼에 그릴 블럭들입니다.
	 */
	void DrawBlocks(const std::vector<Block>& blocks);


private:
	/**
	 * @brief 보드의 기준 좌표입니다.
	 */
	DirectX::XMFLOAT3 basePosition_;


	/**
	 * @brief 보드에서 생성할 수 있는 세로 블럭 수입니다.
	 */
	int32_t countRowBlock_ = 0;


	/**
	 * @brief 보드에서 생성할 수 있는 가로 블럭 수입니다.
	 */
	int32_t countColBlock_ = 0;


	/**
	 * @brief 보드 내의 백그라운드 블럭입니다.
	 */
	std::vector<Block> backgroundBlocks_;


	/**
	 * @brief 보드 내의 외곽선 블럭입니다.
	 */
	std::vector<Block> outlineBlocks_;


	/**
	 * @brief 블럭의 정적 메시입니다.
	 */
	StaticMesh* blockStaticMesh_ = nullptr;


	/**
	 * @brief 블럭의 색상에 대응하는 텍스처입니다.
	 */
	std::unordered_map<Block::EColor, Texture2D*> mappingBlockColorToTextures_;
};