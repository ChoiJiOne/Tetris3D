#include <array>

#include "Button.h"
#include "Label.h"
#include "WorldManager.h"
#include "StartScene.h"

void StartScene::Tick(float deltaSeconds)
{
}

void StartScene::Entry()
{
	SetActive(true);

	uiUpdateOrder_ = 4;

	Label* titleLabel = reinterpret_cast<Label*>(WorldManager::Get().GetGameObject("TitleLabel"));
	if (!titleLabel)
	{
		Label::ConstructorParam titleParam{
			uiUpdateOrder_,
			true,
			DirectX::XMFLOAT2(0.0f, 0.4f),
			"SeoulNamsanEB128",
			L"TETRIS 3D",
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
		};

		WorldManager::Get().AddGameObject("TitleLabel", std::make_unique<Label>(titleParam));
	}
	else
	{
		titleLabel->SetActive(true);
	}

	Button* startButton = reinterpret_cast<Button*>(WorldManager::Get().GetGameObject("StartButton"));
	if (!startButton)
	{
		Button::ConstructorParam startButtonParam{
			uiUpdateOrder_,
			true,
			DirectX::XMFLOAT2(0.0f, 0.0f),
			200.0f,
			200.0f,
			"Start",
			1.0f,
			0.5f,
			0.95f,
			[&]() {
				Leave();
			}
		};

		WorldManager::Get().AddGameObject("StartButton", std::make_unique<Button>(startButtonParam));
	}
	else
	{
		startButton->SetActive(true);
	}

	Button* quitButton = reinterpret_cast<Button*>(WorldManager::Get().GetGameObject("QuitButton"));
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

		WorldManager::Get().AddGameObject("QuitButton", std::make_unique<Button>(quitButtonParam));
	}
	else
	{
		quitButton->SetActive(true);
	}
}

void StartScene::Leave()
{
	std::array<std::string, 4> signatures = {
		"TitleLabel",
		"StartButton",
		"QuitButton",
		"StartScene",
	};

	for (const auto& signature : signatures)
	{
		GameObject* object = WorldManager::Get().GetGameObject(signature);
		object->SetActive(false);
	}

	nextScene_->SetActive(true);
	nextScene_->Entry();
}
