#pragma once

#include <vector>
#include <list>
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
	 * @brief 보드 생성자 파라미터의 구조체입니다.
	 */
	struct ConstructorParam
	{
		int32_t updateOrder;
		bool bIsActive;
		DirectX::XMFLOAT3 basePosition;
		float blockSize;
		Block::EColor blockColor;
		int32_t countRowBlock;
		int32_t countColBlock;
	};


public:
	/**
	 * @brief 보드의 생성자입니다.
	 * 
	 * @param constructorParam 보드의 생성자 파라미터입니다.
	 */
	Board(ConstructorParam&& constructorParam);


	/**
	 * @brief 보드의 생성자입니다.
	 * 
	 * @param constructorParam 보드의 생성자 파라미터입니다.
	 */
	Board(const ConstructorParam& constructorParam);


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
	 * @brief 외곽 라인 내에 블럭들을 추가합니다.
	 * 
	 * @note 추가할 수 없다면 아무 동작도 수행하지 않습니다.
	 * 
	 * @param addBlocks 추가할 블럭들입니다.
	 */
	void AddBlocks(const std::vector<Block>& addBlocks);


	/**
	 * @brief 외곽 라인 내에 블럭들을 추가할 수 있는지 확인합니다.
	 * 
	 * @param addBlocks 추가할 수 있는지 확인할 블럭들입니다.
	 * 
	 * @return 모두 추가할 수 있다면 true, 그렇지 않다면 false를 반환합니다.
	 */
	bool IsAddBlocks(const std::vector<Block>& addBlocks);
	

	/**
	 * @brief 보드의 외곽 라인 블럭들을 얻습니다.
	 * 
	 * @return 외곽 라인 블럭의 벡터를 반환합니다.
	 */
	const std::vector<Block>& GetOutlineBlocks() const { return outlineBlocks_; }


	/**
	 * @brief 보드의 외곽 라인 내의 블럭들을 얻습니다.
	 * 
	 * @return 외곽 라인 내 블럭의 벡터를 반환합니다.
	 */
	const std::list<Block>& GetInnerBlocks() const { return innerBlocks_; }


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
	 * @brief y좌표에 해당하는 라인이 꽉 차있는지 확인합니다.
	 * 
	 * @param y 확인할 라인의 y좌표입니다.
	 * 
	 * @return 해당 라인이 꽉 차있다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool IsFillLine(float y);


	/**
	 * @brief y좌표에 해당하는 라인이 비어있는지 확인합니다.
	 * 
	 * @param y 확인할 라인의 y좌표입니다.
	 *
	 * @return 해당 라인이 비어있다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool IsEmptyLine(float y);


	/**
	 * @brief y좌표에 해당하는 라인을 지웁니다.
	 * 
	 * @param y 지울 라인의 y좌표입니다.
	 */
	void RemoveLine(float y);


	/**
	 * @brief 채워져 있는 라인의 y좌표 목록을 얻습니다.
	 * 
	 * @return 채워져 있는 라인의 y좌표 목록을 반환합니다.
	 */
	std::vector<float> FindFillLines();


	/**
	 * @brief 비워져있는 라인의 y좌표 목록을 얻습니다.
	 * 
	 * @return 비워져있는 라인의 y좌표 목록을 반환합니다.
	 */
	std::vector<float> FindEmptyLines();


	/**
	 * @brief 블럭이 적어도 하나 존재하는 라인의 y좌표 목록을 얻습니다.
	 * 
	 * @return 블럭이 적어도 하나 존재하는 라인의 y좌표 목록을 반환합니다.
	 */
	std::vector<float> FindExistLines();


	/**
	 * @brief 특정 라인에 있는 블럭을 다른 라인으로 이동시킵니다.
	 * 
	 * @param fromY 블럭들을 옮길 특정 블럭들 라인의 y좌표 입니다.
	 * @param toY 블럭들이 이동할 라인의 y좌표 입니다.
	 */
	void MoveLine(float fromY, float toY);
	

	/**
	 * @brief 외곽선 내 블럭들 중 비어있는 부분을 채웁니다.
	 */
	void FillEmptyLine();


	/**
	 * @brief 블럭들을 백버퍼에 그립니다.
	 * 
	 * @param blocks 백버퍼에 그릴 블럭들입니다.
	 * @param alpha 블럭들의 투명도입니다.
	 */
	void DrawBlocks(const std::vector<Block>& blocks, float alpha);


	/**
	 * @brief 내부 블럭들을 백버퍼에 그립니다.
	 * 
	 * @param blocks 백버퍼에 그릴 블럭들입니다.
	 * @param alpha 블럭들의 투명도입니다.
	 */
	void DrawInnerBlocks(const std::list<Block>& blocks, float alpha);


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
	 * @brief 외곽선 블럭 내에 존재하는 블럭들입니다.
	 */
	std::list<Block> innerBlocks_;


	/**
	 * @brief 외곽선 블럭 내에 블럭이 추가되었는지 확인합니다.
	 */
	bool bIsDetectAddBlocks_ = false;


	/**
	 * @brief 삭제한 라인 수입니다.
	 */
	int32_t removeLine_ = 0;


	/**
	 * @brief 블럭의 정적 메시입니다.
	 */
	StaticMesh* blockStaticMesh_ = nullptr;


	/**
	 * @brief 블럭의 색상에 대응하는 텍스처입니다.
	 */
	std::unordered_map<Block::EColor, Texture2D*> mappingBlockColorToTextures_;
};