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


	/**
	 * @brief 테트로미노의 이동 방향입니다.
	 */
	enum class EMovement : int32_t
	{
		NONE     = 0x00,
		LEFT     = 0x01,
		RIGHT    = 0x02,
		UP       = 0x03,
		DOWN     = 0x04,
		CCW      = 0x05,
		CW       = 0x06,
	};


public:
	/**
	 * @brief 테트로미노의 생성자입니다.
	 * 
	 * @param updateOrder 테트로미노의 업데이트 순위입니다.
	 * @param bIsActive 테트로미노의 활성화 여부입니다.
	 * @param basePosition 테트로미노의 기준 좌표입니다.
	 * @param blockSize 테트로미노 블럭의 크기입니다.
	 * @param blockColor 테트로미노 블럭의 색상입니다.
	 */
	Tetromino(
		int32_t updateOrder, 
		bool bIsActive,
		const DirectX::XMFLOAT3& basePosition,
		const EShape& shape,
		float blockSize, 
		const Block::EColor& blockColor
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


	/**
	 * @brief 테트로미노의 기준 좌표를 특정 위치로 이동시킵니다.
	 *
	 * @param basePosition 테트로미노를 이동 시킬 특정 기준 좌표입니다.
	 */
	void Teleport(const DirectX::XMFLOAT3& basePosition);


	/**
	 * @brief 테트로미노의 블럭들을 얻습니다.
	 * 
	 * @return 테트로미노의 블럭들을 저장한 벡터의 참조자를 반환합니다.
	 */
	const std::vector<Block>& GetBlocks() const { return blocks_; }


private:
	/**
	 * @brief 테트로미노 모양에 대응하는 블럭들을 생성합니다.
	 * 
	 * @param shape 테트로미노의 모양입니다.
	 * @param basePosition 테트로미노의 기준 좌표입니다.
	 * @param blockSize 테트로미노 블럭의 크기입니다.
	 * @param blockColor 테트로미노 블럭의 색상입니다.
	 * @parma outRotatePosition[out] 테트로미노의 회전 중심 좌표입니다.
	 * @param outBlocks[out] 테트로미노의 블럭들입니다.
	 */
	void GenerateShapeBlocks(
		const EShape& shape, 
		const DirectX::XMFLOAT3& basePosition, 
		float blockSize, 
		const Block::EColor& blockColor,
		DirectX::XMFLOAT3& outRotatePosition, 
		std::vector<Block>& outBlocks
	);


	/**
	 * @brief 테트로미노를 업데이트합니다.
	 */
	void Update();


	/**
	 * @brief 테트로미노의 블럭들을 백버퍼에 렌더링을 수행합니다.
	 * 
	 * @param blocks 백버퍼에 그릴 블럭들입니다.
	 */
	void DrawBlocks(const std::vector<Block>& blocks);
	

	/**
	 * @brief 테트로미노를 이동시킵니다.
	 * 
	 * @param movement 테트로미노를 이동시킬 방향입니다.
	 */
	void Move(const EMovement& movement);


	/**
	 * @brief 테트로미노가 특정 블럭들과 충돌하는지 확인합니다.
	 * 
	 * @param checkBlocks 충돌하는지 확인할 블럭들입니다.
	 * 
	 * @return 특정 블럭들 중 하나라도 충돌이 감지된다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool IsCollisionBlocks(const std::vector<Block>& checkBlocks) const;


	/**
	 * @brief 테트로미가 움직일 방향의 반대 방향을 얻습니다.
	 * 
	 * @param movement 반대 방향을 얻을 테트로미노 이동 방향입니다.
	 * 
	 * @return 테트로미노 이동 방향의 반대 방향을 반환합니다.
	 */
	EMovement GetCountMovement(const EMovement& movement) const;
	

private:
	/**
	 * @brief 테트로미노의 모양입니다.
	 */
	EShape shape_ = EShape::NONE;


	/**
	 * @brief 테트로미노의 기준 좌표입니다.
	 */
	DirectX::XMFLOAT3 basePosition_;


	/**
	 * @brief 테트로미노의 회전시 기준 좌표입니다.
	 */
	DirectX::XMFLOAT3 rotatePosition_;


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