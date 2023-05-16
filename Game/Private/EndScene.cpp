#include <array>

#include "Board.h"
#include "Button.h"
#include "ContentManager.h"
#include "Label.h"
#include "StringHelper.hpp"
#include "Sound.h"
#include "PlayScene.h"
#include "WorldManager.h"
#include "EndScene.h"

void EndScene::Tick(float deltaSeconds)
{
}

void EndScene::Entry()
{
	SetActive(true);

	uiUpdateOrder_ = 4;
	removeLine_ = reinterpret_cast<Board*>(WorldManager::Get().GetGameObject("Board"))->GetRemoveLine();
	playTime_ = reinterpret_cast<PlayScene*>(WorldManager::Get().GetGameObject("PlayScene"))->GetPlayTime();

	Label* endTitle = reinterpret_cast<Label*>(WorldManager::Get().GetGameObject("EndTitle"));
	if (!endTitle)
	{
		Label::ConstructorParam endTitleParam{
			uiUpdateOrder_,
			true,
			DirectX::XMFLOAT2(0.0f, 0.6f),
			"SeoulNamsanEB128",
			L"GAME OVER",
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
		};

		WorldManager::Get().AddGameObject("EndTitle", std::make_unique<Label>(endTitleParam));
	}
	else
	{
		endTitle->SetActive(true);
	}

	Label* showResult = reinterpret_cast<Label*>(WorldManager::Get().GetGameObject("ShowResult"));
	if (!showResult)
	{
		Label::ConstructorParam showResultParam{
			uiUpdateOrder_,
			true,
			DirectX::XMFLOAT2(0.0f, 0.4f),
			"SeoulNamsanEB32",
			StringHelper::Format(L"TIME : %3d LINE : %3d", static_cast<int32_t>(playTime_), removeLine_),
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
		};

		WorldManager::Get().AddGameObject("ShowResult", std::make_unique<Label>(showResultParam));
	}
	else
	{
		showResult->SetText(StringHelper::Format(L"TIME : %3d LINE : %3d", static_cast<int32_t>(playTime_), removeLine_));
		showResult->SetActive(true);
	}

	Button* okButton = reinterpret_cast<Button*>(WorldManager::Get().GetGameObject("OkButton"));
	if (!okButton)
	{
		Button::ConstructorParam okButtonParam{
			uiUpdateOrder_,
			true,
			DirectX::XMFLOAT2(0.0f, 0.0f),
			200.0f,
			200.0f,
			"Ok",
			1.0f,
			0.5f,
			0.95f,
			[&]() {
				Leave();
			}
		};

		WorldManager::Get().AddGameObject("OkButton", std::make_unique<Button>(okButtonParam));
	}
	else
	{
		okButton->SetActive(true);
	}

	Button* quitButton = reinterpret_cast<Button*>(WorldManager::Get().GetGameObject("EndQuitButton"));
	if (!quitButton)
	{
		Button::ConstructorParam quitButtonParam{
			uiUpdateOrder_,
			true,
			DirectX::XMFLOAT2(0.0f, -0.6f),
			200.0f,
			200.0f,
			"Quit",
			1.0f,
			0.5f,
			0.95f,
			[&]() {
				if (quitEvent_)
				{
					quitEvent_();
				}
			}
		};

		WorldManager::Get().AddGameObject("EndQuitButton", std::make_unique<Button>(quitButtonParam));
	}
	else
	{
		quitButton->SetActive(true);
	}

	Sound* gameOverSound = ContentManager::Get().GetSound("GameOver");
	gameOverSound->Reset();
	gameOverSound->Play();
}

void EndScene::Leave()
{
	std::array<std::string, 5> signatures = {
		"EndTitle",
		"ShowResult",
		"OkButton",
		"EndQuitButton",
		"EndScene",
	};

	for (const auto& signature : signatures)
	{
		GameObject* object = WorldManager::Get().GetGameObject(signature);
		object->SetActive(false);
	}

	Sound* gameOverSound = ContentManager::Get().GetSound("GameOver");
	gameOverSound->Stop();

	SetActive(false);
	nextScene_->Entry();
}