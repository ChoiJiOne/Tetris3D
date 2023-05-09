#include "Board.h"
#include "UIWidget.h"
#include "StringHelper.hpp"
#include "Tetromino.h"
#include "WorldManager.h"
#include "PlayScene.h"

void PlayScene::Tick(float deltaSeconds)
{
	if (GetTetromino(currentTetrominoID_)->GetState() == Tetromino::EState::END)
	{
		int32_t prevTetrominoID = currentTetrominoID_++;
		DestroyTetromino(prevTetrominoID);

		Tetromino* currentTetromino = GetTetromino(currentTetrominoID_);

		if (!currentTetromino->CanTeleport(tetrominoStartPosition_))
		{
			// 임시 코드. 게임 플레이가 종료되면 오브젝트 비활성화
			SetActive(false);
			currentTetromino->SetActive(false);

			Board* board = reinterpret_cast<Board*>(WorldManager::Get().GetGameObject("Board"));
			board->SetActive(false);
			// 임시 코드. 게임 플레이가 종료되면 오브젝트 비활성화
		}
		else
		{
			currentTetromino->Teleport(tetrominoStartPosition_);
			currentTetromino->SetState(Tetromino::EState::RUNNING);
			GenerateTetromino(currentTetrominoID_ + 1, tetrominoWaitPosition_);
		}
	}
}

void PlayScene::Entry()
{
	tetrominoUpdateOrder_ = 2;
	boardUpdateOrder_ = 3;
	blockSize_ = 2.0f;
	tetrominoMaxAccumulatedTime_ = 1.0f;
	currentTetrominoID_ = 0;

	boardBasePosition_ = DirectX::XMFLOAT3(-11.0f, 21.0f, 0.0f);
	tetrominoStartPosition_ = DirectX::XMFLOAT3(-1.0f, 19.0f, 0.0f);
	tetrominoWaitPosition_ = DirectX::XMFLOAT3(15.0f, 11.0f, 0.0f);

	boardRowCount_ = 22;
	boardColCount_ = 12;
	
	Board::ConstructorParam boardParam {
		boardUpdateOrder_,
		true,
		boardBasePosition_,
		blockSize_,
		Block::EColor::GRAY,
		boardRowCount_,
		boardColCount_
	};

	WorldManager::Get().AddGameObject("Board", std::make_unique<Board>(boardParam));

	GenerateTetromino(currentTetrominoID_, tetrominoStartPosition_);
	GenerateTetromino(currentTetrominoID_ + 1, tetrominoWaitPosition_);

	Tetromino* currentTetromino = GetTetromino(currentTetrominoID_);
	currentTetromino->SetState(Tetromino::EState::RUNNING);
}

void PlayScene::Leave()
{
	WorldManager::Get().RemoveGameObject("UIWidget");
	WorldManager::Get().RemoveGameObject("Board");
	DestroyTetromino(currentTetrominoID_);
}

void PlayScene::GenerateTetromino(int32_t tetrominoID, const DirectX::XMFLOAT3& position)
{
	Tetromino::ConstructorParam param{
		tetrominoUpdateOrder_,
		true,
		position,
		Tetromino::GetRandomShape(),
		blockSize_,
		Block::GetRandomColor(),
		tetrominoMaxAccumulatedTime_
	};

	WorldManager::Get().AddGameObject(StringHelper::Format("tetromino%d", tetrominoID), std::make_unique<Tetromino>(param));
}

Tetromino* PlayScene::GetTetromino(int32_t tetrominoID)
{
	std::string signature = StringHelper::Format("tetromino%d", tetrominoID);

	return reinterpret_cast<Tetromino*>(WorldManager::Get().GetGameObject(signature));
}

void PlayScene::DestroyTetromino(int32_t tetrominoID)
{
	std::string signature = StringHelper::Format("tetromino%d", tetrominoID);

	WorldManager::Get().RemoveGameObject(signature);
}
