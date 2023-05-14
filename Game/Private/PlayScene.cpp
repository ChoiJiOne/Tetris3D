#include "Board.h"
#include "Button.h"
#include "ContentManager.h"
#include "StringHelper.hpp"
#include "Label.h"
#include "Tetromino.h"
#include "WorldManager.h"
#include "PlayScene.h"

void PlayScene::Tick(float deltaSeconds)
{
	Tetromino* currentTetromino = GetTetromino(currentTetrominoID_);
	if (currentTetromino->GetState() == Tetromino::EState::END)
	{
		int32_t prevTetrominoID = currentTetrominoID_++;
		DestroyTetromino(prevTetrominoID);

		Tetromino* nextTetromino = GetTetromino(currentTetrominoID_);

		if (!nextTetromino->CanTeleport(tetrominoStartPosition_))
		{
			// 임시 코드. 게임 플레이가 종료되면 오브젝트 비활성화
			SetActive(false);
			nextTetromino->SetActive(false);

			Board* board = GetBoard();
			board->SetActive(false);
			// 임시 코드. 게임 플레이가 종료되면 오브젝트 비활성화
		}
		else
		{
			nextTetromino->Teleport(tetrominoStartPosition_);
			nextTetromino->SetState(Tetromino::EState::RUNNING);
			GenerateTetromino(currentTetrominoID_ + 1, tetrominoWaitPosition_);
		}
	}

	timer_.Tick();
	Label* timeLabel = reinterpret_cast<Label*>(WorldManager::Get().GetGameObject("TimeLabel"));
	timeLabel->SetText(StringHelper::Format(L"TIME : %3d", static_cast<int32_t>(timer_.GetTotalSeconds())));
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

	uiUpdateOrder_ = 4;

	Button::ConstructorParam playButtonParam{
		uiUpdateOrder_,
		true,
		DirectX::XMFLOAT2(-0.8f, 0.8f),
		50.0f,
		50.0f,
		"Stop",
		1.0f,
		0.5f,
		0.95f,
		[&]() {
			Tetromino* currentTetromino = GetTetromino(currentTetrominoID_);
			Button* button = reinterpret_cast<Button*>(WorldManager::Get().GetGameObject("PlayButton"));

			if (currentTetromino->GetState() == Tetromino::EState::READY)
			{
				timer_.Start();
				currentTetromino->SetState(Tetromino::EState::RUNNING);
				button->SetTexture(ContentManager::Get().GetTexture2D("Stop"));
			}
			else if (currentTetromino->GetState() == Tetromino::EState::RUNNING)
			{
				timer_.Stop();
				currentTetromino->SetState(Tetromino::EState::READY);
				button->SetTexture(ContentManager::Get().GetTexture2D("Play"));
			}
		}
	};

	WorldManager::Get().AddGameObject("PlayButton", std::make_unique<Button>(playButtonParam));

	Button::ConstructorParam soundButtonParam{
		uiUpdateOrder_,
		true,
		DirectX::XMFLOAT2(-0.8f, 0.6f),
		50.0f,
		50.0f,
		"Mute",
		1.0f,
		0.5f,
		0.95f,
		[&]() {
		}
	};

	WorldManager::Get().AddGameObject("SoundButton", std::make_unique<Button>(soundButtonParam));

	Label::ConstructorParam timeLabelParam {
		uiUpdateOrder_,
		true,
		DirectX::XMFLOAT2(0.6f, -0.0f),
		"SeoulNamsanEB32",
		StringHelper::Format(L"TIME : %3d", 0),
		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	};

	WorldManager::Get().AddGameObject("TimeLabel", std::make_unique<Label>(timeLabelParam));

	Label::ConstructorParam lineLabelParam {
		uiUpdateOrder_,
		true,
		DirectX::XMFLOAT2(0.6f, -0.2f),
		"SeoulNamsanEB32",
		StringHelper::Format(L"LINE : %3d", 0),
		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	};

	WorldManager::Get().AddGameObject("LineLabel", std::make_unique<Label>(lineLabelParam));

	timer_.Reset();
}

void PlayScene::Leave()
{
	WorldManager::Get().RemoveGameObject("PlayButton");
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

Board* PlayScene::GetBoard()
{
	return reinterpret_cast<Board*>(WorldManager::Get().GetGameObject("Board"));;
}
