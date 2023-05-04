#include <array>

#include "MathHelper.hpp"
#include "StringHelper.hpp"
#include "WorldManager.h"
#include "TetrominoTracker.h"

TetrominoTracker::TetrominoTracker(
	int32_t updateOrder, 
	bool bIsActive, 
	int32_t tetrominoUpdateOrder, 
	float blockSize,
	float tetrominoMaxAccumulatedTime,
	const DirectX::XMFLOAT3& startPosition,
	const DirectX::XMFLOAT3& waitPosition
) : GameObject(updateOrder, bIsActive)
, tetrominoUpdateOrder_(tetrominoUpdateOrder)
, blockSize_(blockSize)
, tetrominoMaxAccumulatedTime_(tetrominoMaxAccumulatedTime)
, startPosition_(startPosition)
, waitPosition_(waitPosition)
{
}

TetrominoTracker::~TetrominoTracker()
{
}

void TetrominoTracker::Tick(float deltaSeconds)
{
	if (currentTetrominoID_ == -1)
	{
		GenerateTetromino(++currentTetrominoID_, startPosition_);
		GenerateTetromino(currentTetrominoID_ + 1, waitPosition_);

		Tetromino* currentTetromino = GetTetromino(currentTetrominoID_);
		currentTetromino->SetState(Tetromino::EState::RUNNING);
	}
	else
	{
		Tetromino* currentTetromino = GetTetromino(currentTetrominoID_);

		if (currentTetromino->GetState() == Tetromino::EState::END)
		{
			int32_t prevTetrominoID = currentTetrominoID_++;
			DestroyTetromino(prevTetrominoID);

			Tetromino* nextTetromino = GetTetromino(currentTetrominoID_);
			nextTetromino->Teleport(startPosition_);
			nextTetromino->SetState(Tetromino::EState::RUNNING);

			GenerateTetromino(currentTetrominoID_ + 1, waitPosition_);
		}
	}
}

Block::EColor TetrominoTracker::GenerateRandomBlockColor() const
{
	static std::array<Block::EColor, 9> blockColors = {
		Block::EColor::BLUE,
		Block::EColor::CYAN,
		Block::EColor::GREEN,
		Block::EColor::MAGENTA,
		Block::EColor::ORANGE,
		Block::EColor::PINK,
		Block::EColor::PURPLE,
		Block::EColor::RED,
		Block::EColor::YELLOW,
	};

	return blockColors[MathHelper::GenerateRandomInt(0, blockColors.size() - 1)];
}

Tetromino::EShape TetrominoTracker::GenerateRandomTetrominoShape() const
{
	static std::array<Tetromino::EShape, 7> tetrominoShapes = {
		Tetromino::EShape::I,
		Tetromino::EShape::O,
		Tetromino::EShape::T,
		Tetromino::EShape::J,
		Tetromino::EShape::L,
		Tetromino::EShape::S,
		Tetromino::EShape::Z,
	};

	return tetrominoShapes[MathHelper::GenerateRandomInt(0, tetrominoShapes.size() - 1)];
}

void TetrominoTracker::GenerateTetromino(int32_t tetrominoID, const DirectX::XMFLOAT3& position)
{
	WorldManager::Get().AddGameObject(
		StringHelper::Format("tetromino%d", tetrominoID),
		std::make_unique<Tetromino>(
			tetrominoUpdateOrder_,
			true,
			position,
			GenerateRandomTetrominoShape(),
			blockSize_,
			GenerateRandomBlockColor(),
			tetrominoMaxAccumulatedTime_
		)
	);
}

Tetromino* TetrominoTracker::GetTetromino(int32_t tetrominoID)
{
	std::string signature = StringHelper::Format("tetromino%d", tetrominoID);
	
	return reinterpret_cast<Tetromino*>(WorldManager::Get().GetGameObject(signature));
}

void TetrominoTracker::DestroyTetromino(int32_t tetrominoID)
{
	std::string signature = StringHelper::Format("tetromino%d", tetrominoID);

	WorldManager::Get().RemoveGameObject(signature);
}